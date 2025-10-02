#include "array_utils.h"
#include <vector>

void find_min_max(const std::vector<int>& arr, int& outMin, int& outMax) {
    if (arr.empty()) {
        outMin = 0;
        outMax = 0;
        return;
    }

    int mn = arr[0];
    int mx = arr[0];
    for (int i = 1; i < arr.size(); ++i) {
        int v = arr[i];
        if (v < mn) mn = v;
        if (v > mx) mx = v;
    }
    outMin = mn;
    outMax = mx;
}

double compute_average(const std::vector<int>& arr) {
    if (arr.empty()) return 0.0;
    long long sum = 0;
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return static_cast<double>(sum) / static_cast<double>(arr.size());
}

void replace_min_max_with_value(std::vector<int>& arr, int newValue) {
    if (arr.empty()) return;

    int mn = arr[0];
    int mx = arr[0];
    for (int i = 1; i < arr.size(); ++i) {
        int v = arr[i];
        if (v < mn) mn = v;
        if (v > mx) mx = v;
    }

    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] == mn || arr[i] == mx) arr[i] = newValue;
    }
}