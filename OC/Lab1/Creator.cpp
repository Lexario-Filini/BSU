#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

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
    int count = std::atoi(argv[2]);
    if (count <= 0) {
        std::cerr << "Count must be positive.\n";
        return 1;
    }

    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Cannot open file for writing: " << filename << "\n";
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        employee e{};
        std::cout << "Enter employee #" << (i + 1) << " (num name hours): ";
        if (!(std::cin >> e.num >> e.name >> e.hours)) {
            std::cerr << "Invalid input.\n";
            return 1;
        }
        e.name[sizeof(e.name) - 1] = '\0';
        out.write(reinterpret_cast<const char*>(&e), sizeof(e));
        if (!out) {
            std::cerr << "Write error.\n";
            return 1;
        }
    }

    std::cout << "Binary file created: " << filename << "\n";
    return 0;
}
