#define NOMINMAX 
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <shared_mutex>
#include <mutex>
#include <atomic>
#include "employee.h"

using namespace std;

const string PIPE_NAME = R"(\\.\pipe\EmployeePipe)";

map<int32_t, shared_mutex> recordLocks;
mutex locksMapMutex;
atomic<int> activeClients{ 0 };
vector<HANDLE> childProcesses;

void print_file(const string& filename) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Failed to open file for reading: " << filename << "\n";
        return;
    }
    cout << "File contents:\n";
    employee e;
    while (fin.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        cout << "ID=" << e.num << " Name=" << e.name << " Hours=" << e.hours << "\n";
    }
    fin.close();
}

bool find_record_offset(const string& filename, int32_t id, streampos& pos, employee& out) {
    ifstream fin(filename, ios::binary);
    if (!fin) return false;
    employee e;
    pos = -1;
    streampos p = 0;
    while (fin.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        if (e.num == id) {
            out = e;
            pos = p;
            fin.close();
            return true;
        }
        p += sizeof(e);
    }
    fin.close();
    return false;
}

void write_record_at(const string& filename, streampos pos, const employee& e) {
    fstream fio(filename, ios::in | ios::out | ios::binary);
    if (!fio) return;
    fio.seekp(pos);
    fio.write(reinterpret_cast<const char*>(&e), sizeof(e));
    fio.flush();
    fio.close();
}

void handle_client(HANDLE pipe, const string& filename) {
    activeClients++;
    cout << "Client connected, thread: " << GetCurrentThreadId() << "\n";
    while (true) {
        int32_t cmd = 0;
        DWORD read = 0;
        BOOL ok = ReadFile(pipe, &cmd, sizeof(cmd), &read, NULL);
        if (!ok || read == 0) break;
        int32_t id = 0;
        ReadFile(pipe, &id, sizeof(id), &read, NULL);
        if (cmd == CLIENT_EXIT) {
            break;
        }
        else if (cmd == READ_REQUEST) {
            {
                lock_guard<mutex> g(locksMapMutex);
                if (recordLocks.find(id) == recordLocks.end()) recordLocks[id];
            }
            recordLocks[id].lock_shared();
            employee e;
            streampos pos;
            int32_t status = 0;
            if (!find_record_offset(filename, id, pos, e)) status = -1;
            DWORD written;
            WriteFile(pipe, &status, sizeof(status), &written, NULL);
            if (status == 0) {
                WriteFile(pipe, &e, sizeof(e), &written, NULL);
            }
            int32_t cmd2 = 0;
            ReadFile(pipe, &cmd2, sizeof(cmd2), &read, NULL);
            recordLocks[id].unlock_shared();
        }
        else if (cmd == WRITE_REQUEST) {
            {
                lock_guard<mutex> g(locksMapMutex);
                if (recordLocks.find(id) == recordLocks.end()) recordLocks[id];
            }
            recordLocks[id].lock();
            employee e;
            streampos pos;
            int32_t status = 0;
            if (!find_record_offset(filename, id, pos, e)) status = -1;
            DWORD written;
            WriteFile(pipe, &status, sizeof(status), &written, NULL);
            if (status == 0) {
                WriteFile(pipe, &e, sizeof(e), &written, NULL);
            }
            int32_t cmd2 = 0;
            DWORD read2 = 0;
            ReadFile(pipe, &cmd2, sizeof(cmd2), &read2, NULL);
            if (cmd2 == WRITE_SUBMIT) {
                employee newe;
                ReadFile(pipe, &newe, sizeof(newe), &read2, NULL);
                if (pos != -1) write_record_at(filename, pos, newe);
                int32_t ack = 0;
                WriteFile(pipe, &ack, sizeof(ack), &written, NULL);
            }
            int32_t cmd3 = 0;
            ReadFile(pipe, &cmd3, sizeof(cmd3), &read2, NULL);
            recordLocks[id].unlock();
        }
        else {
            break;
        }
    }
    FlushFileBuffers(pipe);
    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);
    activeClients--;
    cout << "Client disconnected, thread: " << GetCurrentThreadId() << "\n";
}

int main() {
    cout << "=== Server ===\n";
    string filename;
    cout << "Enter binary file name (for example employees.bin): ";
    getline(cin, filename);
    ofstream fout(filename, ios::binary | ios::trunc);
    if (!fout) {
        cerr << "Failed to create file\n";
        return 1;
    }
    int n;
    cout << "How many records to add? ";
    cin >> n;
    for (int i = 0; i < n; ++i) {
        employee e;
        cout << "Record " << i + 1 << " ID: ";
        cin >> e.num;
        cout << "Name (no spaces, max 9 chars): ";
        string name; cin >> name;
        strncpy(e.name, name.c_str(), sizeof(e.name) - 1);
        cout << "Hours: "; cin >> e.hours;
        fout.write(reinterpret_cast<char*>(&e), sizeof(e));
    }
    fout.close();
    print_file(filename);
    int clientsCount;
    cout << "How many client processes to start? ";
    cin >> clientsCount;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<thread> workerThreads;
    thread listener([&]() {
        for (int i = 0; i < clientsCount; ++i) {
            HANDLE pipe = CreateNamedPipeA(
                PIPE_NAME.c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                4096, 4096,
                0,
                NULL
            );
            if (pipe == INVALID_HANDLE_VALUE) {
                cerr << "CreateNamedPipe failed\n";
                return;
            }
            BOOL connected = ConnectNamedPipe(pipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
            if (connected) {
                workerThreads.emplace_back(handle_client, pipe, filename);
            }
            else {
                CloseHandle(pipe);
            }
        }
        });
    string clientPath;
    cout << "Enter path to client executable (client.exe) or leave empty if in the same folder: ";
    getline(cin, clientPath);
    if (clientPath.empty()) clientPath = "client.exe";
    for (int i = 0; i < clientsCount; ++i) {
        STARTUPINFOA si; PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        string cmd = clientPath;
        BOOL ok = CreateProcessA(
            NULL,
            cmd.data(),
            NULL,
            NULL,
            FALSE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi
        );
        if (!ok) {
            cerr << "Failed to start client: " << cmd << "\n";
        }
        else {
            childProcesses.push_back(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
    if (!childProcesses.empty()) {
        WaitForMultipleObjects((DWORD)childProcesses.size(), childProcesses.data(), TRUE, INFINITE);
    }
    for (auto& t : workerThreads) if (t.joinable()) t.join();
    if (listener.joinable()) listener.join();
    cout << "All clients finished. Modified file:\n";
    print_file(filename);
    cout << "Press Enter to exit server...";
    string dummy; getline(cin, dummy);
    return 0;
}
