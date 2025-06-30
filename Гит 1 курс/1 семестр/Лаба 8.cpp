#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

int* readArrayFromFile(const string& filename, int& size) {
    ifstream inFile(filename);
    if (!inFile) {
       cerr << "Не удалось открыть файл для чтения: " << filename << endl;
        return nullptr;
    }
    inFile >> size;
    int* arr = new int[size];
    for (int i = 0; i < size; ++i) {
        inFile >> arr[i];
    }
    inFile.close();
    return arr;
}
int maxArray(int* arr, int size) {
    if (size == 1) {
        return arr[0];
    }
    int max_of_rest = maxArray(arr + 1, size - 1);
    return (arr[0] > max_of_rest) ? arr[0] : max_of_rest;
}
int main() {
    setlocale(LC_ALL, "RUS");
    string inputFilename = "input.txt";
    string outputFilename = "output.txt";
    int size;

    int* arr = readArrayFromFile(inputFilename, size);
    if (!arr) {
        return 1;
    }
    int max = maxArray(arr, size);

    ofstream outFile(outputFilename);
    if (!outFile) {
        cerr << "Не удалось открыть файл для записи: " << outputFilename << endl;
        delete[] arr;
        return 1;
    }
    cout << "Резльтаты записаны в файл.";
    outFile << "Максимальный элемент: " << max << endl;

    delete[] arr;
    outFile.close();
    return 0;
}




