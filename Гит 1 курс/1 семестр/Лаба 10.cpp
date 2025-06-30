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
        cout << "����:\n";
        cout << "1. ����������� ��������� ������\n";
        cout << "2. ������� ��������� ��������� � �������� �������\n";
        cout << "3. �������� ��������� ��������� � �������� ������� c �������\n";
        cout << "4. �������� ��������� ��������� �� �������\n";
        cout << "5. ����� � ������� ����� ����� ���������������������, ������ ������� ������� ����� �������� ������� �������\n";
        cout << "6. ����� ����������� ���������������� ���������\n";
        cout << "7. ����� �� ���������\n";
        cout << "������� ��� �����: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "��������� ������: ";
            printList(myList);
            break;
        case 2:
            cout << "������� ������� ��� ��������: ";
            cin >> pos;
            cout << "������� ���������� ��������� ��� ��������: ";
            cin >> count;
            deleteElements(myList, pos, count);
            break;
        case 3:
            cout << "������� ������� ��� �������: ";
            cin >> pos;
            insertElements(myList, pos, array);
            break;
        case 4:
            myList.insert(myList.end(), array.begin(), array.end());
            break;
        case 5:
            if (searchSubsequence(myList, array)) {
                cout << "��������������������� �������.\n";
            }
            else {
                cout << "��������������������� �� �������.\n";
            }
            break;
        case 6:
        {
            auto it = findSortedRange(myList);
            cout << "���������� ��������������� ��������: ";
            copy(myList.begin(), it, ostream_iterator<int>(cout, " "));
            cout << endl;
        }
        break;
        case 7:
            return 0;
        default:
            cout << "�������� �����. ���������� ��� ���.\n";
            break;
        }
    }
}
