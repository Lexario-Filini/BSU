#pragma once
#include <windows.h>
#include <iostream>
#include <string>

const int MSG_SIZE = 20;

struct Message {
    char text[MSG_SIZE];
};

struct FileHeader {
    int head;
    int tail;
    int capacity;
};

const std::wstring MUTEX_NAME = L"Global\\FileAccessMutex";
const std::wstring SEM_EMPTY_NAME = L"Global\\SemEmptySlots";
const std::wstring SEM_FULL_NAME = L"Global\\SemFilledSlots";
const std::wstring SEM_READY_NAME = L"Global\\SemSendersReady";
