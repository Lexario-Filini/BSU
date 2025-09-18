#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <cstring>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};

void printBinaryFile(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open binary file.\n";
        return;
    }

    employee emp;
    std::cout << "Содержимое бинарного файла:\n";
    while (in.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        std::cout << emp.num << "\t" << emp.name << "\t" << emp.hours << "\n";
    }
    in.close();
}

void printTextFile(const char* filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open report file.\n";
        return;
    }

    std::cout << "\nОтчет:\n";
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << "\n";
    }
    in.close();
}

bool runProcess(const std::string& command) {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    char* cmdLine = new char[command.size() + 1];
    std::strcpy(cmdLine, command.c_str());

    BOOL success = CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    if (!success) {
        std::cerr << "Ошибка запуска процесса: " << command << "\n";
        std::cerr << "Код ошибки: " << GetLastError() << "\n";
        delete[] cmdLine;
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    delete[] cmdLine;
    return true;
}

int main() {
    setlocale(LC_ALL, "RUS");
    std::string binFile, reportFile;
    int count;
    double rate;

    std::cout << "Enter the name of the binary file: ";
    std::cin >> binFile;
    std::cout << "Enter the number of records: ";
    std::cin >> count;

    std::string creatorCmd = "Creator.exe " + binFile + " " + std::to_string(count);
    if (!runProcess(creatorCmd)) return 1;

    printBinaryFile(binFile.c_str());

    std::cout << "\nEnter the report file name: ";
    std::cin >> reportFile;
    std::cout << "Enter your hourly rate: ";
    std::cin >> rate;

    std::string reporterCmd = "Reporter.exe " + binFile + " " + reportFile + " " + std::to_string(rate);
    if (!runProcess(reporterCmd)) return 1;

    printTextFile(reportFile.c_str());

    return 0;
}
