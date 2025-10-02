#include "threadHandler.h"
#include <iostream>

DWORD WINAPI MinMaxThreadProc(LPVOID lpParam) {
    ThreadArg* arg = static_cast<ThreadArg*>(lpParam);
    if (!arg || !arg->arrPtr) {
        std::cout << "min_max: invalid argument\n";
        delete arg;
        return 0;
    }

    const std::vector<int>& arr = *arg->arrPtr;
    if (arr.empty()) {
        std::cout << "min_max: array is empty\n";
        delete arg->arrPtr;
        delete arg;
        return 0;
    }

    int mn = arr[0];
    int mx = arr[0];

    for (size_t i = 0; i < arr.size(); ++i) {
        int v = arr[i];

        if (v < mn) mn = v;
        Sleep(7);

        if (v > mx) mx = v;
        Sleep(7);
    }

    std::cout << "min_max: min = " << mn << " max = " << mx << "\n";

    delete arg->arrPtr;
    delete arg;
    return 0;
}

DWORD WINAPI AverageThreadProc(LPVOID lpParam) {
    ThreadArg* arg = static_cast<ThreadArg*>(lpParam);
    if (!arg || !arg->arrPtr) {
        std::cout << "average: invalid argument\n";
        delete arg;
        return 0;
    }

    const std::vector<int>& arr = *arg->arrPtr;
    if (arr.empty()) {
        std::cout << "average: array is empty\n";
        delete arg->arrPtr;
        delete arg;
        return 0;
    }

    long long sum = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        sum += arr[i];
        Sleep(12);
    }

    double avg = static_cast<double>(sum) / static_cast<double>(arr.size());
    std::cout << "average: " << avg << "\n";

    delete arg->arrPtr;
    delete arg;
    return 0;
}

void start_min_max_thread(const std::vector<int>& arr, HANDLE& outThreadHandle) {
    ThreadArg* arg = new ThreadArg;
    arg->arrPtr = new std::vector<int>(arr);

    outThreadHandle = CreateThread(nullptr, 0, MinMaxThreadProc, arg, 0, nullptr);
    if (!outThreadHandle) {
        delete arg->arrPtr;
        delete arg;
    }
}

void start_average_thread(const std::vector<int>& arr, HANDLE& outThreadHandle) {
    ThreadArg* arg = new ThreadArg;
    arg->arrPtr = new std::vector<int>(arr);

    outThreadHandle = CreateThread(nullptr, 0, AverageThreadProc, arg, 0, nullptr);
    if (!outThreadHandle) {
        delete arg->arrPtr;
        delete arg;
    }
}
