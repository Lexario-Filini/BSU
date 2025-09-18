#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: Reporter <binfile> <reportfile> <rate>\n";
        return 1;
    }

    const char* binfile = argv[1];
    const char* reportfile = argv[2];
    double rate = std::atof(argv[3]);

    if (rate < 0) {
        std::cerr << "Rate must be non-negative.\n";
        return 1;
    }

    std::ifstream in(binfile, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open binary file: " << binfile << "\n";
        return 1;
    }

    std::ofstream out(reportfile);
    if (!out) {
        std::cerr << "Cannot open report file: " << reportfile << "\n";
        return 1;
    }

    out << "Отчет по файлу \"" << binfile << "\"\n";
    out << "Номер\tИмя\tЧасы\tЗарплата\n";

    employee e{};
    out << std::fixed << std::setprecision(2);
    while (in.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        double salary = e.hours * rate;
        out << e.num << "\t" << e.name << "\t" << e.hours << "\t" << salary << "\n";
    }

    std::cout << "Report created: " << reportfile << "\n";
    return 0;
}
