#ifndef MATRIX_H
#define MATRIX_H



#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

class Matrix {
private:
    vector<vector<double>> data;
    size_t rows;
    size_t cols;

public:
    // Конструкторы
    Matrix(size_t rows, size_t cols);
    Matrix(const Matrix& other);
    ~Matrix();

    // Перегрузка операторов
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    Matrix operator/(double scalar) const;

    // Постфиксные и префиксные инкременты/декременты
    Matrix& operator++();    // префиксный
    Matrix operator++(int);  // постфиксный
    Matrix& operator--();    // префиксный
    Matrix operator--(int);  // постфиксный

    // Операторы сравнения
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    bool operator>=(const Matrix& other) const;
    bool operator<=(const Matrix& other) const;

    // Индексация
   vector<double> operator[](size_t index) const;

    // Дружественные функции для перегрузки операторов
    friend ostream& operator<<(ostream& os, const Matrix& matrix);
    friend istream& operator>>(istream& is, Matrix& matrix);

    // Метод для добавления единичной матрицы
   // Matrix operator++(int);  // сложение с единичной матрицей
};

#endif // MATRIX_H


// Конструктор
Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    data.resize(rows, vector<double>(cols, 0.0));
}

// Конструктор копирования
Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols), data(other.data) {}

// Деструктор
Matrix::~Matrix() {}

// Перегрузка операторов
Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw invalid_argument("Matrices must have the same dimensions for addition.");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices must have the same dimensions for subtraction.");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}

Matrix Matrix::operator/(double scalar) const {
    if (scalar == 0) {
        throw invalid_argument("Division by zero.");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] / scalar;
        }
    }
    return result;
}

// Постфиксный инкремент
Matrix Matrix::operator++(int) {
    Matrix temp = *this;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j]++;
        }
    }
    return temp;
}

// Префиксный инкремент
Matrix& Matrix::operator++() {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j]++;
        }
    }
    return *this;
}

// Постфиксный декремент
Matrix Matrix::operator--(int) {
    Matrix temp = *this;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j]--;
        }
    }
    return temp;
}

// Префиксный декремент
Matrix& Matrix::operator--() {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j]--;
        }
    }
    return *this;
}

// Операторы сравнения
bool Matrix::operator==(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) return false;

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            if (data[i][j] != other.data[i][j]) return false;

    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

bool Matrix::operator>=(const Matrix& other) const {
    return this->data[0][0] >= other.data[0][0]; // Пример для простоты
}

bool Matrix::operator<=(const Matrix& other) const {
    return this->data[0][0] <= other.data[0][0]; // Пример для простоты
}

// Индексация
vector<double> Matrix::operator[](size_t index) const {
    if (index >= rows) throw out_of_range("Index out of range.");

    return data[index];
}

// Дружественные функции для перегрузки операторов ввода-вывода
ostream& operator<<(ostream& os, const Matrix& matrix) {
    for (const auto& row : matrix.data) {
        for (const auto& elem : row) {
            os << elem << " ";
        }
        os << "\n";
    }
    return os;
}

istream& operator>>(istream& is, Matrix& matrix) {
    for (size_t i = 0; i < matrix.rows; ++i)
        for (size_t j = 0; j < matrix.cols; ++j)
            is >> matrix.data[i][j];

    return is;
}


int main() {
    try {
        Matrix mat1(2, 2);
        Matrix mat2(2, 2);

        cout << "Enter elements of matrix 1:\n";
        cin >> mat1;

        cout << "Enter elements of matrix 2:\n";
        cin >> mat2;

        cout << "Matrix 1:\n" << mat1;
        cout << "Matrix 2:\n" << mat2;

        // Сложение матриц
        Matrix mat3 = mat1 + mat2;
        cout << "Result of addition:\n" << mat3;

        // Вычитание матриц
        Matrix mat4 = mat1 - mat2;
        cout << "Result of subtraction:\n" << mat4;

        // Умножение на число
        double scalar = 2.0;
        cout << "Result of multiplication by " << scalar << ":\n" << (mat1 * scalar);

        // Деление на число
        cout << "Result of division by " << scalar << ":\n" << (mat1 / scalar);

        // Инкременты
        cout << "Incrementing matrix 1:\n" << ++mat1;

        // Индексация
        cout << "First row of matrix 1:\n";
        auto row = mat1[0];
        for (auto elem : row) {
            cout << elem << " ";
        }
        cout << "\n";

        // Сравнение матриц
        if (mat1 == mat2)
            cout << "Matrices are equal.\n";
        else
            cout << "Matrices are not equal.\n";

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
