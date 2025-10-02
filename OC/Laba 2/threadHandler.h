#pragma once

#include <windows.h>
#include <vector>

struct ThreadArg {
    std::vector<int>* arrPtr; 
};

DWORD WINAPI MinMaxThreadProc(LPVOID lpParam);
DWORD WINAPI AverageThreadProc(LPVOID lpParam);

void start_min_max_thread(const std::vector<int>& arr, HANDLE& outThreadHandle);
void start_average_thread(const std::vector<int>& arr, HANDLE& outThreadHandle);
