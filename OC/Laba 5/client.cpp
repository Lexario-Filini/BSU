#include <windows.h>
#include <iostream>
#include <string>
#include "employee.h"

using namespace std;

const string PIPE_NAME = R"(\\.\pipe\EmployeePipe)";

bool connectPipe(HANDLE& pipe) {
    while (true) {
        pipe = CreateFileA(
            PIPE_NAME.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );
        if (pipe != INVALID_HANDLE_VALUE) break;
        if (GetLastError() != ERROR_PIPE_BUSY) {
            cerr << "Failed to open pipe. Error: " << GetLastError() << "\n";
            return false;
        }
        if (!WaitNamedPipeA(PIPE_NAME.c_str(), 5000)) {
            cerr << "Pipe wait timeout\n";
            return false;
        }
    }
    return true;
}

int main() {
    cout << "=== Client ===\n";
    HANDLE pipe;
    if (!connectPipe(pipe)) return 1;
    cout << "Connected to server\n";
    while (true) {
        cout << "\nChoose operation:\n1 - read record\n2 - modify record\n3 - exit\nYour choice: ";
        int choice; cin >> choice;
        if (choice == 3) {
            int32_t cmd = CLIENT_EXIT;
            int32_t id = 0;
            DWORD written;
            WriteFile(pipe, &cmd, sizeof(cmd), &written, NULL);
            WriteFile(pipe, &id, sizeof(id), &written, NULL);
            break;
        }
        else if (choice == 1) {
            cout << "Enter ID to read: ";
            int32_t id; cin >> id;
            int32_t cmd = READ_REQUEST;
            DWORD written, read;
            WriteFile(pipe, &cmd, sizeof(cmd), &written, NULL);
            WriteFile(pipe, &id, sizeof(id), &written, NULL);
            int32_t status = 0;
            ReadFile(pipe, &status, sizeof(status), &read, NULL);
            if (status != 0) {
                cout << "Record with ID=" << id << " not found\n";
                int32_t rel = RELEASE;
                WriteFile(pipe, &rel, sizeof(rel), &written, NULL);
                continue;
            }
            employee e;
            ReadFile(pipe, &e, sizeof(e), &read, NULL);
            cout << "Received record: ID=" << e.num << " Name=" << e.name << " Hours=" << e.hours << "\n";
            cout << "Press Enter to release access to the record...";
            cin.ignore(); cin.get();
            int32_t rel = RELEASE;
            WriteFile(pipe, &rel, sizeof(rel), &written, NULL);
        }
        else if (choice == 2) {
            cout << "Enter ID to modify: ";
            int32_t id; cin >> id;
            int32_t cmd = WRITE_REQUEST;
            DWORD written, read;
            WriteFile(pipe, &cmd, sizeof(cmd), &written, NULL);
            WriteFile(pipe, &id, sizeof(id), &written, NULL);
            int32_t status = 0;
            ReadFile(pipe, &status, sizeof(status), &read, NULL);
            if (status != 0) {
                cout << "Record with ID=" << id << " not found\n";
                int32_t rel = RELEASE;
                WriteFile(pipe, &rel, sizeof(rel), &written, NULL);
                continue;
            }
            employee e;
            ReadFile(pipe, &e, sizeof(e), &read, NULL);
            cout << "Current record: ID=" << e.num << " Name=" << e.name << " Hours=" << e.hours << "\n";
            cout << "Enter new values:\n";
            cout << "Name: ";
            string name; cin >> name;
            cout << "Hours: ";
            double h; cin >> h;
            employee newe = e;
            strncpy(newe.name, name.c_str(), sizeof(newe.name) - 1);
            newe.hours = h;
            cout << "Send modified record to server? (1 - yes / 0 - no): ";
            int sendNow; cin >> sendNow;
            if (sendNow) {
                int32_t sub = WRITE_SUBMIT;
                WriteFile(pipe, &sub, sizeof(sub), &written, NULL);
                WriteFile(pipe, &newe, sizeof(newe), &written, NULL);
                int32_t ack = 0;
                ReadFile(pipe, &ack, sizeof(ack), &read, NULL);
                cout << "Server confirmed write\n";
            }
            else {
                int32_t dummy = 0;
                WriteFile(pipe, &dummy, sizeof(dummy), &written, NULL);
            }
            cout << "Press Enter to release access to the record...";
            cin.ignore(); cin.get();
            int32_t rel = RELEASE;
            WriteFile(pipe, &rel, sizeof(rel), &written, NULL);
        }
        else {
            cout << "Invalid choice\n";
        }
    }
    CloseHandle(pipe);
    cout << "Client exiting\n";
    return 0;
}
