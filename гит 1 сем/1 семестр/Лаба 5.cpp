#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;
void printMatrix(double matrix[][10], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(10) << setprecision(2) << fixed << matrix[i][j] << " ";
        }
        cout << endl;
    }
}
bool invertMatrix(double matrix[][10], double inverse[][10], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                inverse[i][j] = 1.0;
            else
                inverse[i][j] = 0.0;
        }
    }
    for (int i = 0; i < n; i++) {
        double pivot = matrix[i][i];
        if (pivot == 0) {
            return false;
        }
        for (int j = 0; j < n; j++) {
            matrix[i][j] /= pivot;
            inverse[i][j] /= pivot;
        }
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = matrix[k][i];
                for (int j = 0; j < n; j++) {
                    matrix[k][j] -= factor * matrix[i][j];
                    inverse[k][j] -= factor * inverse[i][j];
                }
            }
        }
    }
    return true;
}
int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    int n;
    cout << "Введите размерность квадратной матрицы (N): ";
    cin >> n;
    if (n <= 0 || n > 10) {
        cout << "Размерность должна быть от 1 до 10." << endl;
        return 1;
    }
    double matrix[10][10];
    double inverse[10][10];
    cout << "Сгенерированная матрица:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (rand()%10);
        }
    }
    printMatrix(matrix, n);
    if (invertMatrix(matrix, inverse, n)) {
        cout << "Обратная матрица:" << endl;
        printMatrix(inverse, n);
    }
    else {
        cout << "Матрица не имеет обратной." << endl;
    }
    return 0;
}

