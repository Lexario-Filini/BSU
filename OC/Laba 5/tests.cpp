#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "employee.h"

bool write_record(const std::string& filename, const employee& e) {
    std::ofstream fout(filename, std::ios::binary | std::ios::app);
    if (!fout) return false;
    fout.write(reinterpret_cast<const char*>(&e), sizeof(e));
    return true;
}

bool read_record(const std::string& filename, int32_t id, employee& out) {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin) return false;
    employee e;
    while (fin.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        if (e.num == id) {
            out = e;
            return true;
        }
    }
    return false;
}

TEST(EmployeeTest, WriteAndReadRecord) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 1;
    strncpy(e.name, "Alex", sizeof(e.name) - 1);
    e.hours = 40.5;

    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, 1, out));
    EXPECT_EQ(out.num, 1);
    EXPECT_STREQ(out.name, "Alex");
    EXPECT_DOUBLE_EQ(out.hours, 40.5);
}

TEST(EmployeeTest, RecordNotFound) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 2;
    strncpy(e.name, "John", sizeof(e.name) - 1);
    e.hours = 20.0;
    ASSERT_TRUE(write_record(filename, e));

    employee out;
    EXPECT_FALSE(read_record(filename, 999, out));
}

TEST(EmployeeTest, MultipleRecords) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e1;
    e1.num = 1;
    strncpy(e1.name, "Alex", sizeof(e1.name) - 1);
    e1.hours = 10.0;

    employee e2;
    e2.num = 2;
    strncpy(e2.name, "John", sizeof(e2.name) - 1);
    e2.hours = 20.0;

    employee e3;
    e3.num = 3;
    strncpy(e3.name, "Kate", sizeof(e3.name) - 1);
    e3.hours = 30.0;

    ASSERT_TRUE(write_record(filename, e1));
    ASSERT_TRUE(write_record(filename, e2));
    ASSERT_TRUE(write_record(filename, e3));

    employee out;
    ASSERT_TRUE(read_record(filename, 2, out));
    EXPECT_EQ(out.num, 2);
    EXPECT_STREQ(out.name, "John");
    EXPECT_DOUBLE_EQ(out.hours, 20.0);
}

TEST(EmployeeTest, EmptyFileRead) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee out;
    EXPECT_FALSE(read_record(filename, 1, out));
}

TEST(EmployeeTest, OverwriteRecord) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 5;
    strncpy(e.name, "First", sizeof(e.name) - 1);
    e.hours = 10.0;
    ASSERT_TRUE(write_record(filename, e));

    employee e2;
    e2.num = 5;
    strncpy(e2.name, "Second", sizeof(e2.name) - 1);
    e2.hours = 99.9;
    ASSERT_TRUE(write_record(filename, e2));

    employee out;
    ASSERT_TRUE(read_record(filename, 5, out));
    EXPECT_STREQ(out.name, "First");
    EXPECT_DOUBLE_EQ(out.hours, 10.0);
}

TEST(EmployeeTest, BoundaryValues) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 10;
    strncpy(e.name, "ABCDEFGHI", sizeof(e.name) - 1); 
    e.hours = 0.0;
    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, 10, out));
    EXPECT_EQ(out.num, 10);
    EXPECT_STREQ(out.name, "ABCDEFGHI");
    EXPECT_DOUBLE_EQ(out.hours, 0.0);
}

TEST(EmployeeTest, SequentialReads) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e1;
    e1.num = 1;
    strncpy(e1.name, "A", sizeof(e1.name) - 1);
    e1.hours = 1.0;

    employee e2;
    e2.num = 2;
    strncpy(e2.name, "B", sizeof(e2.name) - 1);
    e2.hours = 2.0;

    employee e3;
    e3.num = 3;
    strncpy(e3.name, "C", sizeof(e3.name) - 1);
    e3.hours = 3.0;

    ASSERT_TRUE(write_record(filename, e1));
    ASSERT_TRUE(write_record(filename, e2));
    ASSERT_TRUE(write_record(filename, e3));

    employee out;
    ASSERT_TRUE(read_record(filename, 1, out));
    EXPECT_EQ(out.num, 1);
    EXPECT_STREQ(out.name, "A");

    ASSERT_TRUE(read_record(filename, 2, out));
    EXPECT_EQ(out.num, 2);
    EXPECT_STREQ(out.name, "B");

    ASSERT_TRUE(read_record(filename, 3, out));
    EXPECT_EQ(out.num, 3);
    EXPECT_STREQ(out.name, "C");
}

TEST(EmployeeTest, DuplicateIDs) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e1;
    e1.num = 100;
    strncpy(e1.name, "First", sizeof(e1.name) - 1);
    e1.hours = 10.0;

    employee e2;
    e2.num = 100;
    strncpy(e2.name, "Second", sizeof(e2.name) - 1);
    e2.hours = 20.0;

    ASSERT_TRUE(write_record(filename, e1));
    ASSERT_TRUE(write_record(filename, e2));

    employee out;
    ASSERT_TRUE(read_record(filename, 100, out));
    EXPECT_STREQ(out.name, "First");
    EXPECT_DOUBLE_EQ(out.hours, 10.0);
}

TEST(EmployeeTest, LargeHoursValue) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 200;
    strncpy(e.name, "Big", sizeof(e.name) - 1);
    e.hours = 123456.789;

    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, 200, out));
    EXPECT_DOUBLE_EQ(out.hours, 123456.789);
}

TEST(EmployeeTest, NegativeHoursValue) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 300;
    strncpy(e.name, "Neg", sizeof(e.name) - 1);
    e.hours = -5.5;

    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, 300, out));
    EXPECT_DOUBLE_EQ(out.hours, -5.5);
}

TEST(EmployeeTest, NameTruncation) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 400;
    strncpy(e.name, "VeryLongNameHere", sizeof(e.name) - 1);
    e.hours = 12.0;

    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, 400, out));
    EXPECT_EQ(out.num, 400);
    EXPECT_EQ(strlen(out.name), 9); 
}

TEST(EmployeeTest, MultipleFilesIsolation) {
    std::string file1 = "test_file1.bin";
    std::string file2 = "test_file2.bin";
    std::remove(file1.c_str());
    std::remove(file2.c_str());

    employee e1;
    e1.num = 1;
    strncpy(e1.name, "File1", sizeof(e1.name) - 1);
    e1.hours = 11.0;

    employee e2;
    e2.num = 2;
    strncpy(e2.name, "File2", sizeof(e2.name) - 1);
    e2.hours = 22.0;

    ASSERT_TRUE(write_record(file1, e1));
    ASSERT_TRUE(write_record(file2, e2));

    employee out1, out2;
    ASSERT_TRUE(read_record(file1, 1, out1));
    ASSERT_TRUE(read_record(file2, 2, out2));

    EXPECT_STREQ(out1.name, "File1");
    EXPECT_STREQ(out2.name, "File2");
}

TEST(EmployeeTest, ZeroID) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 0;
    strncpy(e.name, "Zero", sizeof(e.name) - 1);
    e.hours = 5.0;
    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, 0, out));
    EXPECT_EQ(out.num, 0);
    EXPECT_STREQ(out.name, "Zero");
    EXPECT_DOUBLE_EQ(out.hours, 5.0);
}

TEST(EmployeeTest, MaxIntID) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = INT32_MAX;
    strncpy(e.name, "MaxID", sizeof(e.name) - 1);
    e.hours = 1.0;
    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, INT32_MAX, out));
    EXPECT_EQ(out.num, INT32_MAX);
    EXPECT_STREQ(out.name, "MaxID");
}

TEST(EmployeeTest, NegativeID) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = -123;
    strncpy(e.name, "NegID", sizeof(e.name) - 1);
    e.hours = 15.0;
    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, -123, out));
    EXPECT_EQ(out.num, -123);
    EXPECT_STREQ(out.name, "NegID");
}

TEST(EmployeeTest, EmptyName) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    employee e;
    e.num = 50;
    strncpy(e.name, "", sizeof(e.name) - 1);
    e.hours = 7.0;
    ASSERT_TRUE(write_record(filename, e));

    employee out;
    ASSERT_TRUE(read_record(filename, 50, out));
    EXPECT_EQ(out.num, 50);
    EXPECT_STREQ(out.name, "");
    EXPECT_DOUBLE_EQ(out.hours, 7.0);
}

TEST(EmployeeTest, MultipleSequentialWritesAndReads) {
    std::string filename = "test_employees.bin";
    std::remove(filename.c_str());

    for (int i = 1; i <= 20; ++i) {
        employee e;
        e.num = i;
        std::string nm = "N" + std::to_string(i);
        strncpy(e.name, nm.c_str(), sizeof(e.name) - 1);
        e.hours = i * 1.5;
        ASSERT_TRUE(write_record(filename, e));
    }

    for (int i = 1; i <= 20; ++i) {
        employee out;
        ASSERT_TRUE(read_record(filename, i, out));
        EXPECT_EQ(out.num, i);
        EXPECT_DOUBLE_EQ(out.hours, i * 1.5);
    }
}
