#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;
string chString(const char* charArray) {
    return string(charArray);
}
void shellSort(char arr[], int n) {
    for (int g = n / 2; g > 0; g /= 2) {
        for (int i = g; i < n; i++) {
            char temp = arr[i];
            int j;

            for (j = i; j >= g && arr[j - g] > temp; j -= g) {
                arr[j] = arr[j - g];
            }
            arr[j] = temp;
        }
    }
}
int octalToDecimal(int sum) {
    int decimalValue = 0;
    int base = 1;
    while (sum > 0) {
        int lastDigit = sum % 10;
        sum /= 10;
        decimalValue += lastDigit * base;
        base *= 8;
    }
    return decimalValue;
}
int main() {
    setlocale(LC_ALL, "");
    ofstream file1("output.txt");
    ifstream file2("input.txt");
    char n[100];
    char r[100];

    if (file2.is_open()) {
        file2.getline(n, 100);
        file2.getline(r, 100);
        file2.close();
    }
    else {
        cerr << "Не удалось открыть файл textin.txt" << endl;
        return 1;
    }
    int l1 = strlen(n);
    int l2 = strlen(r);

    if (file1.is_open()) {
        file1 << "Разбитие на лексемы" << endl;

        for (int i = 0; i < l1; i++) {
            int y = 0;
            while (i < l1) {
                bool found = false;
                for (int j = 0; j < l2; j++) {
                    if (r[j] == n[i]) {
                        y = 1;
                        i++;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    break;
                }
            }
            if (y == 1 && i < l1) {
                file1 << endl;
            }
            if (i < l1) {
                file1 << n[i];
            }
        }
        file1 << endl;
        file1 << "8-ые числа изменены на 10-ые" << endl;

        for (int i = 0; i < l1; i++) {
            int y = 0;
            int sum = 0;
            bool b = false;

            while (isdigit(n[i]) && n[i] != '9' && n[i] != '8' && !b) {
                for (int h = 0; h < l2; h++) {
                    if (n[i] == r[h]) {
                        b = true;
                        i++;
                        break;
                    }
                }
                if (!b) {
                    sum = sum * 10 + (n[i] - '0');
                    i++;
                }
            }

            if (sum > 0) {
                int octal = octalToDecimal(sum);
                file1 << octal;
            }

            while (true) {
                bool found = false;
                for (int j = 0; j < l2; j++) {
                    if (r[j] == n[i]) {
                        y = 1;
                        i++;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    break;
                }
            }

            if (y == 1 || b) {
                file1 << endl;
            }

            if (i < l1) {
                file1 << n[i];
            }
        }
        file1 << endl;

        file1 << "Сортировка Шелла" << endl;
        shellSort(r, l2);
        for (int i = 0; i < l2; i++) {
            file1 << r[i];
        }
        file1 << endl;

        file1 << "Задание 2" << endl;

        char w3[100];
        char w4[100];
        for (int i = 0; i < 100; i++) {
            w3[i] = n[i];
        }

        int y = 0, c = 0, u = 0, u1 = 0, i1 = 0;
        for (int i = 0; i < l1; i++) {
            if (y == 0 && (isdigit(w3[i]) && w3[i] != '9' && w3[i] != '8')) {
                y = 1;
                c++;
            }
            if (c == 2 && (isdigit(w3[i]) && w3[i] != '9' && w3[i] != '8')) {
                u = i;
            }
            while ((isdigit(w3[i]) && w3[i] != '9' && w3[i] != '8')) {
                i++;
            }
            if (c == 2) {
                u1 = u; i1 = i;
                for (; i < l1; i++) {
                    w3[u] = w3[i];
                    u++;
                }
            }
            if (c == 2) {
                break;
            }
            y = 0;
        }
        for (int i = 0; i < l1 - (i1 - u1); i++) {
            w4[i] = w3[i];
        }
        w4[l1 - (i1 - u1)] = '\0';

        int l3 = strlen(w4);
        string w2 = chString(w4);
        srand(time(0));
        int ran = rand() % 100;
        string ran2 = to_string(ran);
        w2.insert(l3 / 2, ran2 + " ");
        file1 << w2 << endl;

        file1.close();
        cout << "Результаты записаны в файл"<< endl;
    }
    else {
        cerr << "Не удалось открыть файл output.txt" << endl;
        return 1;
    }
    return 0;
}