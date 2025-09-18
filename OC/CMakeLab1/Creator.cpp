#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: Creator <filename> <count>\n";
        return 1;
    }

    const char* filename = argv[1];
    int count = std::stoi(argv[2]);

    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Cannot open file for writing.\n";
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        employee emp;
        std::cout << "Enter an employee #" << (i + 1) << " (num name hours): ";
        std::cin >> emp.num >> emp.name >> emp.hours;
        out.write(reinterpret_cast<char*>(&emp), sizeof(emp));
    }

    out.close();
    return 0;
}
