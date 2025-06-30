#include "list.h"
using namespace std;

List::List() : head(nullptr), tail(nullptr) {}

List::~List() {
    Clear();
}

void List::readFromfile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл.");
    }

    string surname, doljnost;
    double salary;
    while (file >> surname >> doljnost >> salary) {
        addTail(surname, doljnost, salary);
    }
    file.close();
}

void List:: addHead(const string& surname, const string& doljnost, int salary) {
    Employee* newNode = new Employee(surname, doljnost, salary);
    if (!head) {
        head = tail = newNode;
    }
    else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
}

void List:: addTail(const string& surname, const string& doljnost, int salary) {
    Employee* newNode = new Employee(surname, doljnost,salary);
    if (!tail) {
        head = tail = newNode;
    }
    else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
}

void List:: addIndex(int index, const string& surname, const string& doljnost,int salary) {
    if (index < 0) throw out_of_range("Некорректный индекс.");

    Employee* newNode = new Employee(surname, doljnost, salary);
    if (index == 0) {
        addHead(surname, doljnost, salary);
        return;
    }

    Employee* current = head;
    for (int i = 0; i < index - 1; ++i) {
        if (!current) throw out_of_range("Индекс вне диапазона.");
        current = current->next;
    }

    if (!current->next) {
        addTail(surname, doljnost, salary);
    }
    else {
        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;
    }
}

void List:: delHead() {
    if (!head) throw runtime_error("Список пуст.");
    Employee* temp = head;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    }
    else { 
        tail = nullptr;
    }
    delete temp;
}

void List:: delTail() {
    if (!tail) throw runtime_error("Список пуст.");
    Employee* temp = tail;
    tail = tail->prev;
    if (tail) {
        tail->next = nullptr;
    }
    else {
        head = nullptr;
    }
    delete temp;
}

void List:: delIndex(int index) {
    if (index < 0 || !head) throw out_of_range("Некорректный индекс или список пуст.");
    if (index == 0) {
        delHead();
        return;
    }

    Employee* current = head;
    for (int i = 0; i < index; ++i) {
        if (!current) throw out_of_range("Индекс вне диапазона.");
        current = current->next;
    }

    if (!current->next) {
        delTail();
    }
    else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
    }
}

void List:: Clear() {
    while (head) {
        delHead();
    }
}

void List:: sortList() {
    if (!head || !head->next) {
        return;
    }

    bool swapped;
    do {
        swapped = false;
        Employee* current = head;
        while (current->next) {
            if (current->salary > current->next->salary) {
                Swap(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

void List:: Swap(Employee* a, Employee* b) {
    string tempSurname = a->surname;
    string tempDoljnost = a->doljnost;
    double tempSalary = a->salary;

    a->surname = b->surname;
    a->doljnost = b->doljnost;
    a->salary = b->salary;

    b->surname = tempSurname;
    b->doljnost = tempDoljnost;
    b->salary = tempSalary;
}

void List:: printConsole() const {
    if (!head)  throw runtime_error("Список пуст.");
    Employee* current = head;
    while (current) {
        cout << current->surname << " " << current->doljnost << " " << current->salary << endl;
        current = current->next;
    }
}

void List:: filePrint(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл.");
    }

    Employee* current = head;
    while (current) {
        file << current->surname << " " << current->doljnost << " " << current->salary << endl;
        current = current->next;
    }
    file.close();
}
