#include "pch.h"
#include "CppUnitTest.h"
#include "employee.h"
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <iomanip>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern bool createBinaryFile(const std::string& filename, int count);
extern bool generateReport(const std::string& binfile, const std::string& reportfile, double rate);

namespace TestApp
{
    TEST_CLASS(EmployeeTests)
    {
    public:

        TEST_METHOD(TestBinaryWriteAndRead)
        {
            const char* filename = "test_employees.bin";
            std::vector<employee> original = {
                {1, "Ivan", 10.5},
                {2, "Anna", 8.0},
                {3, "Oleg", 12.25}
            };

            {
                std::ofstream out(filename, std::ios::binary);
                Assert::IsTrue(out.is_open(), L"Failed to open file for writing");
                for (const auto& e : original) {
                    out.write(reinterpret_cast<const char*>(&e), sizeof(e));
                }
            }

            {
                std::ifstream in(filename, std::ios::binary);
                Assert::IsTrue(in.is_open(), L"Failed to open file for reading");
                employee e{};
                int index = 0;
                while (in.read(reinterpret_cast<char*>(&e), sizeof(e))) {
                    Assert::AreEqual(original[index].num, e.num);
                    Assert::AreEqual(std::string(original[index].name), std::string(e.name));
                    Assert::AreEqual(original[index].hours, e.hours);
                    ++index;
                }
                Assert::AreEqual((int)original.size(), index);
            }

            std::remove(filename);
        }

        TEST_METHOD(TestReportGeneration)
        {
            const char* binfile = "test_employees.bin";
            const char* reportfile = "test_report.txt";
            double rate = 100.0;

            employee e = { 42, "Test", 5.0 };
            {
                std::ofstream out(binfile, std::ios::binary);
                out.write(reinterpret_cast<const char*>(&e), sizeof(e));
            }

            {
                std::ifstream in(binfile, std::ios::binary);
                std::ofstream out(reportfile);
                Assert::IsTrue(in.is_open(), L"Failed to open binary file");
                Assert::IsTrue(out.is_open(), L"Failed to open report file");

                out << "Отчет по файлу \"" << binfile << "\"\n";
                out << "Номер\tИмя\tЧасы\tЗарплата\n";
                out << std::fixed << std::setprecision(2);
                while (in.read(reinterpret_cast<char*>(&e), sizeof(e))) {
                    double salary = e.hours * rate;
                    out << e.num << "\t" << e.name << "\t" << e.hours << "\t" << salary << "\n";
                }
            }

            {
                std::ifstream in(reportfile);
                Assert::IsTrue(in.is_open(), L"Failed to open report file for validation");

                std::string line;
                std::getline(in, line);
                std::getline(in, line);
                std::getline(in, line);
                Assert::IsTrue(line.find("42") != std::string::npos);
                Assert::IsTrue(line.find("Test") != std::string::npos);
                Assert::IsTrue(line.find("500.00") != std::string::npos);
            }

            std::remove(binfile);
            std::remove(reportfile);
        }
    };
}
