#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};
std::string getExeDirectory() {
    char path[MAX_PATH]{};
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    std::string fullPath(path);
    size_t pos = fullPath.find_last_of("\\/");
    return (pos == std::string::npos) ? "." : fullPath.substr(0, pos);
}

bool runProcessAndWait(const std::string& commandLine) {
    STARTUPINFOA si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};

    std::string cmdCopy = commandLine;
    char* cmdBuffer = &cmdCopy[0]; 

    BOOL success = CreateProcessA(
        nullptr,            
        cmdBuffer,         
        nullptr, nullptr,   
        FALSE,              
        0,                  
        nullptr,            
        nullptr,          
        &si, &pi            
    );

    if (!success) {
        std::cerr << "Ошибка запуска процесса. Код: " << GetLastError() << "\n";
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

void printBinaryFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Не удалось открыть бинарный файл: " << filename << "\n";
        return;
    }

    employee e{};
    std::cout << "\nСодержимое бинарного файла:\n";
    while (in.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        std::cout << e.num << "\t" << e.name << "\t" << e.hours << "\n";
    }
    in.close();
}

void printReportFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Не удалось открыть файл отчета: " << filename << "\n";
        return;
    }

    std::cout << "\nСодержимое отчета:\n";
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << "\n";
    }
    in.close();
}

int main() {
    setlocale(LC_ALL, "RUS");
    SetConsoleOutputCP(65001); 
    std::string binFile, reportFile;
    int count;
    double rate;

    std::cout << "Введите имя бинарного файла: ";
    std::cin >> binFile;
    std::cout << "Введите количество записей: ";
    std::cin >> count;

    std::string exeDir = getExeDirectory();
    std::string creatorPath = "\"" + exeDir + "\\Creator.exe\"";
    std::string creatorCmd = creatorPath + " \"" + binFile + "\" " + std::to_string(count);

    if (!runProcessAndWait(creatorCmd)) return 1;

    printBinaryFile(binFile);

    std::cout << "\nВведите имя файла отчета: ";
    std::cin >> reportFile;
    std::cout << "Введите оплату за час: ";
    std::cin >> rate;

    std::string reporterPath = "\"" + exeDir + "\\Reporter.exe\"";
    std::string reporterCmd = reporterPath + " \"" + binFile + "\" \"" + reportFile + "\" " + std::to_string(rate);

    if (!runProcessAndWait(reporterCmd)) return 1;

    printReportFile(reportFile);

    std::cout << "\nГотово.\n";
    return 0;
}
