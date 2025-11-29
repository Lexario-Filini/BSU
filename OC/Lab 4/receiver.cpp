#define NOMINMAX
#include "common.h"
#include <vector>
#include <fstream>
#include <string>
#include <limits>

int wmain() {
    std::wcout << L"=== Receiver Process ===\n";

    std::wstring filename;
    int capacity = 0;

    std::wcout << L"Enter binary filename: ";
    std::wcin >> filename;
    std::wcout << L"Enter capacity (number of records): ";
    std::wcin >> capacity;

    if (capacity <= 0) {
        std::wcerr << L"Invalid capacity.\n";
        return 1;
    }

    std::fstream file(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        std::wcerr << L"Failed to create file.\n";
        return 1;
    }

    FileHeader header;
    header.head = 0;
    header.tail = 0;
    header.capacity = capacity;
    file.write((char*)&header, sizeof(FileHeader));

    Message emptyMsg;
    for (int i = 0; i < MSG_SIZE; ++i) {
        emptyMsg.text[i] = '\0';
    }
    for (int i = 0; i < capacity; ++i) {
        file.write((char*)&emptyMsg, sizeof(Message));
    }
    file.flush();
    file.close();

    HANDLE hMutex = CreateMutexW(NULL, FALSE, MUTEX_NAME.c_str());
    HANDLE hSemEmpty = CreateSemaphoreW(NULL, capacity, capacity, SEM_EMPTY_NAME.c_str());
    HANDLE hSemFull = CreateSemaphoreW(NULL, 0, capacity, SEM_FULL_NAME.c_str());
    HANDLE hSemReady = CreateSemaphoreW(NULL, 0, 100, SEM_READY_NAME.c_str());

    if (hMutex == NULL || hSemEmpty == NULL || hSemFull == NULL || hSemReady == NULL) {
        std::wcerr << L"Synchronization init failed.\n";
        if (hMutex != NULL) CloseHandle(hMutex);
        if (hSemEmpty != NULL) CloseHandle(hSemEmpty);
        if (hSemFull != NULL) CloseHandle(hSemFull);
        if (hSemReady != NULL) CloseHandle(hSemReady);
        return 1;
    }

    int senderCount = 0;
    std::wcout << L"Enter number of Sender processes: ";
    std::wcin >> senderCount;

    if (senderCount <= 0) {
        std::wcerr << L"Invalid sender count.\n";
        CloseHandle(hMutex);
        CloseHandle(hSemEmpty);
        CloseHandle(hSemFull);
        CloseHandle(hSemReady);
        return 1;
    }

    std::vector<STARTUPINFO> si(senderCount);
    std::vector<PROCESS_INFORMATION> pi(senderCount);

    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    std::wstring exePath(buffer);
    std::wstring dirPath = exePath.substr(0, exePath.find_last_of(L"\\/"));
    std::wstring senderExe = dirPath + L"\\Sender.exe";
    std::wstring senderCmd = L"\"" + senderExe + L"\" \"" + filename + L"\"";

    for (int i = 0; i < senderCount; ++i) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        std::vector<wchar_t> cmd(senderCmd.begin(), senderCmd.end());
        cmd.push_back(L'\0');

        BOOL created = CreateProcessW(NULL, cmd.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]);
        if (created == FALSE) {
            std::wcerr << L"Failed to create Sender process.\n";
            for (int j = 0; j < i; ++j) {
                CloseHandle(pi[j].hProcess);
                CloseHandle(pi[j].hThread);
            }
            CloseHandle(hMutex);
            CloseHandle(hSemEmpty);
            CloseHandle(hSemFull);
            CloseHandle(hSemReady);
            return 1;
        }
    }

    std::wcout << L"Waiting for all Senders to be ready...\n";
    for (int i = 0; i < senderCount; ++i) {
        WaitForSingleObject(hSemReady, INFINITE);
    }
    std::wcout << L"All Senders are ready!\n";

    while (true) {
        std::wcout << L"\nOptions:\n1. Read message\n2. Exit\nChoice: ";
        int choice = 0;
        if (!(std::wcin >> choice)) {
            std::wcin.clear();
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
            continue;
        }
        if (choice == 2) {
            break;
        }
        if (choice == 1) {
            std::wcout << L"Waiting for message...\n";

            DWORD waitFull = WaitForSingleObject(hSemFull, INFINITE);
            if (waitFull != WAIT_OBJECT_0) {
                std::wcerr << L"Wait on full slot failed.\n";
                continue;
            }

            DWORD waitMutex = WaitForSingleObject(hMutex, INFINITE);
            if (waitMutex != WAIT_OBJECT_0) {
                std::wcerr << L"Wait on mutex failed.\n";
                ReleaseSemaphore(hSemFull, 1, NULL);
                continue;
            }

            std::fstream inFile(filename, std::ios::in | std::ios::out | std::ios::binary);
            if (!inFile.is_open()) {
                ReleaseMutex(hMutex);
                std::wcerr << L"Failed to open file for reading.\n";
                ReleaseSemaphore(hSemEmpty, 1, NULL);
                continue;
            }

            inFile.seekg(0, std::ios::beg);
            inFile.read((char*)&header, sizeof(FileHeader));

            int offset = (int)sizeof(FileHeader) + (header.head * (int)sizeof(Message));
            inFile.seekg(offset, std::ios::beg);

            Message msg;
            inFile.read((char*)&msg, sizeof(Message));

            int wlen = MultiByteToWideChar(CP_UTF8, 0, msg.text, -1, NULL, 0);
            std::wstring wide_str;
            if (wlen > 0) {
                wide_str.resize(wlen - 1);
                MultiByteToWideChar(CP_UTF8, 0, msg.text, -1, &wide_str[0], wlen);
            }
            std::wcout << L"Received: " << wide_str << std::endl;

            header.head = (header.head + 1) % header.capacity;
            inFile.seekp(0, std::ios::beg);
            inFile.write((char*)&header, sizeof(FileHeader));

            inFile.flush();
            inFile.close();

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hSemEmpty, 1, NULL);
        }
    }

    CloseHandle(hMutex);
    CloseHandle(hSemEmpty);
    CloseHandle(hSemFull);
    CloseHandle(hSemReady);

    for (int i = 0; i < senderCount; ++i) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

    return 0;
}
