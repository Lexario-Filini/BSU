#include <iostream>
#include <vector>
#include <windows.h>
#include "array_utils.h"
#include "threadHandler.h"

int main() {
    std::cout << "Enter array size: ";
    int n;
    if (!(std::cin >> n) || n < 0) {
        std::cerr << "Invalid size\n";
        return 1;
    }

    std::vector<int> arr;
    arr.reserve(n);
    std::cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; ++i) {
        int x;
        if (!(std::cin >> x)) {
            std::cerr << "Invalid input\n";
            return 1;
        }
        arr.push_back(x);
    }

    HANDLE hMinMax = nullptr;
    HANDLE hAverage = nullptr;

    start_min_max_thread(arr, hMinMax);
    start_average_thread(arr, hAverage);

    if (hMinMax) {
        WaitForSingleObject(hMinMax, INFINITE);
        CloseHandle(hMinMax);
        hMinMax = nullptr;
    }
    if (hAverage) {
        WaitForSingleObject(hAverage, INFINITE);
        CloseHandle(hAverage);
        hAverage = nullptr;
    }

    double avg = compute_average(arr);
    int avgInt = static_cast<int>(avg + (avg >= 0 ? 0.5 : -0.5));

    replace_min_max_with_value(arr, avgInt);

    std::cout << "Resulting array:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << (i + 1 < n ? ' ' : '\n');
    }
    std::cin.get();
    std::cin.get();
    std::cin.get();
    return 0;
}
