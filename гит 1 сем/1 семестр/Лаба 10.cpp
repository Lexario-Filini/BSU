#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;
void printList(const list<int>& lst) {
    copy(lst.begin(), lst.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}
int generateRandomNumber() {
    return rand() % 100;
}
void deleteElements(list<int>& lst, int position, int count) {
    auto it = lst.begin();
    advance(it, position);
    for (int i = 0; i < count && it != lst.end(); ++i) {
        it = lst.erase(it);
    }
}
void insertElements(list<int>& lst, int position, const vector<int>& elements) {
    auto it = lst.begin();
    advance(it, position);
    lst.insert(it, elements.begin(), elements.end());
}
bool searchSubsequence(const list<int>& lst, const vector<int>& pattern) {
    vector<int> squares(pattern.size());
    transform(pattern.begin(), pattern.end(), squares.begin(), [](int n) { return n * n; });
    return search(lst.begin(), lst.end(), squares.begin(), squares.end()) != lst.end();
}
list<int>::iterator findSortedRange(list<int>& lst) {
    return is_sorted_until(lst.begin(), lst.end());
}

int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(0));

    list<int> myList = { 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 };
    vector<int> array = { 2, 3, 4 };
    int choice, pos, count;

    while (true) {
        cout << "Меню:\n";
        cout << "1. Просмотреть состояние списка\n";
        cout << "2. Удалить несколько элементов с заданной позиции\n";
        cout << "3. Добавить несколько элементов в заданную позицию c массива\n";
        cout << "4. Добавить несколько элементов из массива\n";
        cout << "5. Найти в массиве целых чисел подпоследовательность, каждый элемент которой равен квадрату другого массива\n";
        cout << "6. Поиск наибольшего отсортированного диапазона\n";
        cout << "7. Выход из программы\n";
        cout << "Введите ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Состояние списка: ";
            printList(myList);
            break;
        case 2:
            cout << "Введите позицию для удаления: ";
            cin >> pos;
            cout << "Введите количество элементов для удаления: ";
            cin >> count;
            deleteElements(myList, pos, count);
            break;
        case 3:
            cout << "Введите позицию для вставки: ";
            cin >> pos;
            insertElements(myList, pos, array);
            break;
        case 4:
            myList.insert(myList.end(), array.begin(), array.end());
            break;
        case 5:
            if (searchSubsequence(myList, array)) {
                cout << "Подпоследовательность найдена.\n";
            }
            else {
                cout << "Подпоследовательность не найдена.\n";
            }
            break;
        case 6:
        {
            auto it = findSortedRange(myList);
            cout << "Наибольший отсортированный диапазон: ";
            copy(myList.begin(), it, ostream_iterator<int>(cout, " "));
            cout << endl;
        }
        break;
        case 7:
            return 0;
        default:
            cout << "Неверный выбор. Попробуйте еще раз.\n";
            break;
        }
    }
}
