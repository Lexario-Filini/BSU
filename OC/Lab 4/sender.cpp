#define NOMINMAX
#include "common.h"
#include <fstream>
#include <limits>

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::wstring s = L"Usage: Sender.exe <filename>\n";
        DWORD written = 0;
        WriteConsoleW(GetStdHandle(STD_ERROR_HANDLE), s.c_str(), (DWORD)s.size(), &written, NULL);
        Sleep(3000);
        return 1;
    }

    std::wstring filename = argv[1];

    HANDLE hMutex = OpenMutexW(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME.c_str());
    HANDLE hSemEmpty = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, SEM_EMPTY_NAME.c_str());
    HANDLE hSemFull = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, SEM_FULL_NAME.c_str());
    HANDLE hSemReady = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, SEM_READY_NAME.c_str());

    if (hMutex == NULL || hSemEmpty == NULL || hSemFull == NULL || hSemReady == NULL) {
        std::wstring s = L"Sync objects not found. Run Receiver first.\n";
        DWORD written = 0;
        WriteConsoleW(GetStdHandle(STD_ERROR_HANDLE), s.c_str(), (DWORD)s.size(), &written, NULL);
        Sleep(5000);
        if (hMutex != NULL) CloseHandle(hMutex);
        if (hSemEmpty != NULL) CloseHandle(hSemEmpty);
        if (hSemFull != NULL) CloseHandle(hSemFull);
        if (hSemReady != NULL) CloseHandle(hSemReady);
        return 1;
    }

    {
        std::wstring s = L"Sender started. Signaling ready...\n";
        DWORD written = 0;
        WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), s.c_str(), (DWORD)s.size(), &written, NULL);
    }
    ReleaseSemaphore(hSemReady, 1, NULL);

    while (true) {
        std::wcout << L"\nOptions:\n1. Send message\n2. Exit\nChoice: ";
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
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
            std::wcout << L"Enter message (max 19 chars): ";
            std::wstring winput;
            if (!std::getline(std::wcin, winput)) {
                continue;
            }
            if (winput.size() >= MSG_SIZE) {
                winput.resize(MSG_SIZE - 1);
            }
            int needed = WideCharToMultiByte(CP_UTF8, 0, winput.c_str(), (int)winput.size(), NULL, 0, NULL, NULL);
            std::string input;
            if (needed > 0) {
                input.resize(needed);
                WideCharToMultiByte(CP_UTF8, 0, winput.c_str(), (int)winput.size(), &input[0], needed, NULL, NULL);
            }

            Message msg;
            for (int i = 0; i < MSG_SIZE; ++i) {
                msg.text[i] = '\0';
            }
            int copyLen = (int)input.size();
            if (copyLen >= MSG_SIZE) {
                copyLen = MSG_SIZE - 1;
            }
            for (int i = 0; i < copyLen; ++i) {
                msg.text[i] = input[i];
            }
            msg.text[MSG_SIZE - 1] = '\0';

            std::wcout << L"Waiting for empty slot...\n";

            DWORD waitEmpty = WaitForSingleObject(hSemEmpty, INFINITE);
            if (waitEmpty != WAIT_OBJECT_0) {
                std::wstring s = L"Wait on empty slot failed.\n";
                DWORD written = 0;
                WriteConsoleW(GetStdHandle(STD_ERROR_HANDLE), s.c_str(), (DWORD)s.size(), &written, NULL);
                continue;
            }

            DWORD waitMutex = WaitForSingleObject(hMutex, INFINITE);
            if (waitMutex != WAIT_OBJECT_0) {
                ReleaseSemaphore(hSemEmpty, 1, NULL);
                std::wstring s = L"Wait on mutex failed.\n";
                DWORD written = 0;
                WriteConsoleW(GetStdHandle(STD_ERROR_HANDLE), s.c_str(), (DWORD)s.size(), &written, NULL);
                continue;
            }

            std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
            if (!file.is_open()) {
                ReleaseSemaphore(hSemEmpty, 1, NULL);
                ReleaseMutex(hMutex);
                std::wstring s = L"ERROR: Failed to open shared file for writing!\n";
                DWORD written = 0;
                WriteConsoleW(GetStdHandle(STD_ERROR_HANDLE), s.c_str(), (DWORD)s.size(), &written, NULL);
                continue;
            }

            FileHeader header;
            file.seekg(0, std::ios::beg);
            file.read((char*)&header, sizeof(FileHeader));

            int offset = (int)sizeof(FileHeader) + (header.tail * (int)sizeof(Message));
            file.seekp(offset, std::ios::beg);
            file.write((char*)&msg, sizeof(Message));

            header.tail = (header.tail + 1) % header.capacity;
            file.seekp(0, std::ios::beg);
            file.write((char*)&header, sizeof(FileHeader));

            file.flush();
            file.close();

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hSemFull, 1, NULL);

            std::wcout << L"Message sent.\n";
        }
    }

    CloseHandle(hMutex);
    CloseHandle(hSemEmpty);
    CloseHandle(hSemFull);
    CloseHandle(hSemReady);

    return 0;
}
