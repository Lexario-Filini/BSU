#define NOMINMAX
#include "gtest/gtest.h"
#include "common.h"
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

const std::string TEST_FILE = "unit_test_data.bin";

class ProcessSyncTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove(TEST_FILE.c_str());
    }

    void TearDown() override {
        std::remove(TEST_FILE.c_str());
    }

    void CreateEmptyFile(int capacity) {
        std::fstream file(TEST_FILE, std::ios::out | std::ios::binary | std::ios::trunc);
        FileHeader header;
        header.head = 0;
        header.tail = 0;
        header.capacity = capacity;
        file.write((char*)&header, sizeof(FileHeader));

        Message emptyMsg = { 0 };
        for (int i = 0; i < capacity; ++i) {
            file.write((char*)&emptyMsg, sizeof(Message));
        }
    }
};

TEST_F(ProcessSyncTest, StructSizeValidation) {
    EXPECT_EQ(sizeof(Message), MSG_SIZE);
    EXPECT_EQ(sizeof(FileHeader), 12);
}

TEST_F(ProcessSyncTest, FileInitializationLogic) {
    int capacity = 5;
    CreateEmptyFile(capacity);

    std::fstream file(TEST_FILE, std::ios::in | std::ios::binary);
    ASSERT_TRUE(file.is_open());

    FileHeader header;
    file.read((char*)&header, sizeof(FileHeader));

    EXPECT_EQ(header.head, 0);
    EXPECT_EQ(header.tail, 0);
    EXPECT_EQ(header.capacity, capacity);

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    size_t expectedSize = sizeof(FileHeader) + (capacity * sizeof(Message));
    EXPECT_EQ(fileSize, expectedSize);
}

TEST_F(ProcessSyncTest, CircularBufferWrapping) {
    int capacity = 3;
    CreateEmptyFile(capacity);

    std::fstream file(TEST_FILE, std::ios::in | std::ios::out | std::ios::binary);

    for (int i = 0; i < 4; ++i) {
        FileHeader header;
        file.seekg(0, std::ios::beg);
        file.read((char*)&header, sizeof(FileHeader));

        int offset = sizeof(FileHeader) + (header.tail * sizeof(Message));
        Message msg;
        std::string text = "Msg" + std::to_string(i);
        std::copy(text.begin(), text.end(), msg.text);
        msg.text[text.size()] = '\0';

        file.seekp(offset, std::ios::beg);
        file.write((char*)&msg, sizeof(Message));

        header.tail = (header.tail + 1) % header.capacity;
        file.seekp(0, std::ios::beg);
        file.write((char*)&header, sizeof(FileHeader));
    }

    file.seekg(0, std::ios::beg);
    FileHeader finalHeader;
    file.read((char*)&finalHeader, sizeof(FileHeader));

    EXPECT_EQ(finalHeader.tail, 1);
}

TEST_F(ProcessSyncTest, MessageReadLogic) {
    int capacity = 2;
    CreateEmptyFile(capacity);

    {
        std::fstream file(TEST_FILE, std::ios::in | std::ios::out | std::ios::binary);
        FileHeader header;
        file.read((char*)&header, sizeof(FileHeader));

        Message msg;
        std::string text = "TestString";
        std::strcpy(msg.text, text.c_str());

        int offset = sizeof(FileHeader) + (header.tail * sizeof(Message));
        file.seekp(offset, std::ios::beg);
        file.write((char*)&msg, sizeof(Message));

        header.tail = (header.tail + 1) % capacity;
        file.seekp(0, std::ios::beg);
        file.write((char*)&header, sizeof(FileHeader));
    }

    {
        std::fstream file(TEST_FILE, std::ios::in | std::ios::out | std::ios::binary);
        FileHeader header;
        file.read((char*)&header, sizeof(FileHeader));

        int offset = sizeof(FileHeader) + (header.head * sizeof(Message));
        file.seekg(offset, std::ios::beg);
        Message readMsg;
        file.read((char*)&readMsg, sizeof(Message));

        EXPECT_STREQ(readMsg.text, "TestString");

        header.head = (header.head + 1) % capacity;
        file.seekp(0, std::ios::beg);
        file.write((char*)&header, sizeof(FileHeader));
    }

    {
        std::fstream file(TEST_FILE, std::ios::in | std::ios::binary);
        FileHeader header;
        file.read((char*)&header, sizeof(FileHeader));
        EXPECT_EQ(header.head, 1);
        EXPECT_EQ(header.tail, 1);
    }
}

TEST_F(ProcessSyncTest, MessageTruncation) {
    std::string longInput = "This string is definitely longer than twenty characters";
    Message msg = { 0 };

    int copyLen = (int)longInput.size();
    if (copyLen >= MSG_SIZE) {
        copyLen = MSG_SIZE - 1;
    }

    for (int i = 0; i < copyLen; ++i) {
        msg.text[i] = longInput[i];
    }
    msg.text[MSG_SIZE - 1] = '\0';

    EXPECT_EQ(std::strlen(msg.text), MSG_SIZE - 1);
    EXPECT_EQ(std::string(msg.text), "This string is defi");
}

TEST_F(ProcessSyncTest, SyncObjectsCreation) {
    HANDLE hMutex = CreateMutexW(NULL, FALSE, MUTEX_NAME.c_str());
    EXPECT_NE(hMutex, (HANDLE)NULL);

    HANDLE hSemEmpty = CreateSemaphoreW(NULL, 10, 10, SEM_EMPTY_NAME.c_str());
    EXPECT_NE(hSemEmpty, (HANDLE)NULL);

    HANDLE hSemFull = CreateSemaphoreW(NULL, 0, 10, SEM_FULL_NAME.c_str());
    EXPECT_NE(hSemFull, (HANDLE)NULL);

    HANDLE hSemReady = CreateSemaphoreW(NULL, 0, 100, SEM_READY_NAME.c_str());
    EXPECT_NE(hSemReady, (HANDLE)NULL);

    if (hMutex) CloseHandle(hMutex);
    if (hSemEmpty) CloseHandle(hSemEmpty);
    if (hSemFull) CloseHandle(hSemFull);
    if (hSemReady) CloseHandle(hSemReady);
}