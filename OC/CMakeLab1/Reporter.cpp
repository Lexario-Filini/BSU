#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: Reporter <input_file> <report_file> <hourly_rate>\n";
        return 1;
    }

    const char* input_file = argv[1];
    const char* report_file = argv[2];
    double rate = std::stod(argv[3]);

    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(report_file);

    if (!in || !out) {
        std::cerr << "Error opening files.\n";
        return 1;
    }

    out << "File report \"" << input_file << "\"\n";
    out << "Number\tName\tHours\tSalary\n";

    employee emp;
    while (in.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        double salary = emp.hours * rate;
        out << emp.num << "\t" << emp.name << "\t" << emp.hours << "\t" << std::fixed << std::setprecision(2) << salary << "\n";
    }

    in.close();
    out.close();
    return 0;
}
