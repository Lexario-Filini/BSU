#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

struct Employee {
    string surname;
    string doljnost;
    int salary;
    Employee* prev;
    Employee* next;

    Employee(string surname, string doljnost, int salary)
        : surname(surname), doljnost(doljnost), salary(salary), prev(nullptr), next(nullptr) {}
};

class List {
private:
    Employee* head;
    Employee* tail;


public:
    List();
    ~List();

    void readFromfile(const string& filename);
    void addHead(const string& surname, const string& doljnost, int salary);
    void addTail(const string& surname, const string& doljnost, int salary);
    void addIndex(int index, const string& surname, const string& doljnost, int salary);

    void delHead();
    void delTail();
    void delIndex(int index);
    void Clear();
    void Swap(Employee* a, Employee* b);

    void sortList();
    void printConsole() const;
    void filePrint(const string& filename) const;

};

#endif