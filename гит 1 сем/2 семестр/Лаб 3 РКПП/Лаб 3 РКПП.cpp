#include <iostream>
#include <fstream>
#include <regex>
#include <string>

using namespace std;

bool isValidDate(const string& date) {
    regex dateRegex(R"(^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/0[0-9]{3}$)");
    if (!regex_match(date, dateRegex)) {
        return false;
    }
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    if (year < 1 || year > 1000) return false;

    if (month == 2) {
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        return (isLeapYear && day <= 29) || (!isLeapYear && day <= 28);
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return day <= 30;
    }
    return day <= 31;
}

int main() {
    setlocale(LC_ALL, "RUS");
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Не удалось открыть файл." << endl;
        return 1;
    }
    string line;
    while (getline(inputFile, line)) {
        if (isValidDate(line)) {
            outputFile << line << " - Верная дата" << endl;
        }
        else {
            outputFile << line << " - Неверная дата" << endl;
        }
    }
    inputFile.close();
    outputFile.close();

    cout << "Результаты проверки записаны в файл output.txt" << endl;
    return 0;
}
