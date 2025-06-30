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
        cout << "1. ������� ������ �� �����\n"
            << "2. �������� �������\n"
            << "3. ������� �������\n"
            << "4. ������� ��� ��������\n"
            << "5. ����������� ������\n"
            << "6. ������� ������ �� �������\n"
            << "7. ������� ������ � ����\n"
            << "0. �����\n"
            << "�������� �����: ";
        cin >> choice;

        try {
            switch (choice) {
            case 1: {
                string filename("input.txt");
                /*cout << "������� ��� �����: ";
                cin >> filename;*/
                list.readFromfile(filename);
                break;
            }
            case 2: {
                int index;
                string surname, doljnost;
                int salary;
                cout << "������� ������ (�������, ���������, ��������): ";
                cin >> surname >> doljnost >> salary;
                cout << "������� ������ (0 - ������, -1 - �����): ";
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
                cout << "������� ������ �������� ��� �������� (0 - ������, -1 - �����): ";
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
                cout << "������ ����" << endl;
                break;
            case 5:
                list.sortList();
                break;
            case 6:
                list.printConsole ();
                
                break;
            case 7: {
                string filename;
                cout << "������� ��� �����: ";
                cin >> filename;
                list.filePrint(filename);
                break;
            }
            case 0:
                cout << "�����..." << endl;
                break;
            default:
                cout << "������������ �����. ���������� �����." << endl;
            }
        }
        catch (const exception& e) {
            cout << "������: " << e.what() << endl;
        }
    } while (choice != 0);

    return 0;
}