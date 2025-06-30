#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include "list.h"

using namespace std;

int main() {
    List list;
    int choice;
    setlocale(LC_ALL, "RUS");
    do {
        cout << "1. Создать список из файла\n"
            << "2. Добавить элемент\n"
            << "3. Удалить элемент\n"
            << "4. Удалить все элементы\n"
            << "5. Сортировать список\n"
            << "6. Вывести список на консоль\n"
            << "7. Вывести список в файл\n"
            << "0. Выход\n"
            << "Выберите опцию: ";
        cin >> choice;

        try {
            switch (choice) {
            case 1: {
                string filename("input.txt");
                /*cout << "Введите имя файла: ";
                cin >> filename;*/
                list.readFromfile(filename);
                break;
            }
            case 2: {
                int index;
                string surname, doljnost;
                int salary;
                cout << "Введите данные (фамилию, должность, зарплату): ";
                cin >> surname >> doljnost >> salary;
                cout << "Введите индекс (0 - начало, -1 - конец): ";
                cin >> index;
                if (index == 0) {
                    list.addHead(surname, doljnost, salary);
                }
                else if (index == -1) {
                    list.addTail(surname, doljnost, salary);
                }
                else {
                    list.addIndex(index, surname, doljnost, salary);
                }
                break;
            }
            case 3: {
                int index;
                cout << "Введите индекс элемента для удаления (0 - начало, -1 - конец): ";
                cin >> index;
                if (index == 0) {
                    list.delHead();
                }
                else if (index == -1) {
                    list.delTail();
                }
                else {
                    list.delIndex(index);
                }
                break;
            }
            case 4:
                list.Clear();
                cout << "Список пуст" << endl;
                break;
            case 5:
                list.sortList();
                break;
            case 6:
                list.printConsole ();
                
                break;
            case 7: {
                string filename;
                cout << "Введите имя файла: ";
                cin >> filename;
                list.filePrint(filename);
                break;
            }
            case 0:
                cout << "Выход..." << endl;
                break;
            default:
                cout << "Некорректный выбор. Попробуйте снова." << endl;
            }
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
    } while (choice != 0);

    return 0;
}