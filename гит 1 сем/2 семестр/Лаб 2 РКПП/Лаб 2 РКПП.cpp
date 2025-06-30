#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

struct Element1 {
    char Name[50];
    int kurs;
    int Otdelenie;
    Element1() {
        Name[0] = '0';
        kurs = 0;
        Otdelenie = 0;
    }
    Element1(int a, int b) {
        Name[0] = '0';
        kurs = a;
        Otdelenie = b;
    }
    friend ostream& operator<<(ostream& out, const Element1& a) {
        out << a.Name << " " << a.kurs << " " << a.Otdelenie;
        return out;
    }
    friend istream& operator>>(istream& in, Element1& a) {
        in.getline(a.Name, sizeof(a.Name));
        in >> a.kurs;
        in >> a.Otdelenie;
        in.ignore();
        return in;
    }
    void writeBinary(ofstream& out) const {
        out.write((char*)this, sizeof(Element1));
    }
    void readBinary(ifstream& in) const {
        in.read((char*)this, sizeof(Element1));
    }
    bool operator==(const Element1& other) const {
        return (strcmp(Name, other.Name)==1 && kurs==other.kurs && Otdelenie==other.Otdelenie);
    }
};

struct Element2 {
    char Name[50];
    int kurs;
    int Otdelenie;
    Element2() {
        Name[0] = '0';
        kurs = 0;
        Otdelenie = 0;
    }
    Element2( int b) {
        Name[0] = '0';
        kurs = 0;
        Otdelenie = b;
    }
    friend ostream& operator<<(ostream& out, const Element2& a) {
        out << a.Name << " " << a.kurs << " " << a.Otdelenie;
        return out;
    }
    friend istream& operator>>(istream& in, Element2& a) {
        in.getline(a.Name, sizeof(a.Name));
        in >> a.kurs;
        in >> a.Otdelenie;
        in.ignore();
        return in;
    }
    void writeBinary(ofstream& out) const {
        out.write((char*)this, sizeof(Element2));
    }
    void readBinary(ifstream& in) const {
        in.read((char*)this, sizeof(Element2));
    }
    bool operator==(const Element2& other) const {
        return (Otdelenie == other.Otdelenie);
    }
};

struct Element3 {
    char Name[50];
    int kurs;
    int Otdelenie;
    Element3() {
        Name[0] = '0';
        kurs = 0;
        Otdelenie = 0;
    }
    friend ostream& operator<<(ostream& out, const Element3& a) {
        out << a.Name << " " << a.kurs<<" "<<a.Otdelenie;
        return out;
    }
    void writeBinary(ofstream& out) const {
        out.write((char*)this, sizeof(Element3));
    }
    Element3& operator=(const Element1& other) {
        strcpy(Name, other.Name);
        kurs = other.kurs;
        Otdelenie = other.Otdelenie;
        return *this;
    }
    Element3& operator=(const Element2& other) {
        strcpy(Name, other.Name);
        kurs = other.kurs;
        Otdelenie = other.Otdelenie;
        return *this;
    }
};

template <typename T>
class MASSIV {
private:
    T* data;
    size_t size;
public:
    MASSIV() : data(nullptr), size(0) {}

    MASSIV(size_t n) : size(n) {
        data = new T[n];
        for (size_t i = 0; i < size; i++) {
            data[i] = T();
        }
    }

    MASSIV(const MASSIV<T>& other) : size(other.size) {
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    ~MASSIV() {
        delete[] data;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            cout << "Index out of range";
            exit(1);
        }
        return data[index];



    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            cout << "Index out of range";
            exit(1);
        }
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    MASSIV<T>& operator=(const MASSIV<T>& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    friend ostream& operator<<(ostream& out, const MASSIV<T>& Arr) {
        for (size_t i = 0; i < Arr.size; i++) {
            out << Arr.data[i] << endl;
        }
        return out;
    }

    friend istream& operator>>(istream& in, MASSIV<T>& Arr) {
        for (size_t i = 0; i < Arr.size; i++) {
            in >> ws;
            in >> Arr.data[i];
        }
        return in;
    }
    void writeBinary(const string& filename) const {
        ofstream out(filename, ios::binary);
        if (out.is_open()) {
            out.write((char*)&size, sizeof(size));
            for (size_t i = 0; i < size; i++) {
                data[i].writeBinary(out);
            }
            out.close();
        }
        else {
            cout << "Unable to open file: " << filename << endl;
        }
    }
    void readBinary(const string& filename) {
        ifstream in(filename, ios::binary);
        if (in.is_open()) {
            in.read((char*)&size, sizeof(size));
            delete[] data;
            data = new T[size];
            for (size_t i = 0; i < size; i++) {
                data[i].readBinary(in);
            }
            in.close();
        }
        else {
            cout << "Unable to open file: " << filename << endl;
        }
    }
};
bool operator==(const Element1& a, const Element2& b) {
    return (strcmp(a.Name, b.Name)==0 && a.kurs == b.kurs && a.Otdelenie == b.Otdelenie);
}
bool operator==(const Element2& a, const Element1& b) {
    return (strcmp(a.Name, b.Name) == 0 && a.kurs == b.kurs && a.Otdelenie == b.Otdelenie);
}
bool operator==(const Element3& a, const Element3& b) {
    return (strcmp(a.Name, b.Name) == 1 && a.kurs == b.kurs && a.Otdelenie == b.Otdelenie);
}
template <typename T3, typename T1, typename T2>
MASSIV<T3> connecter(const MASSIV<T1>& Arr1, const MASSIV<T2>& Arr2) {
    vector<T3> result;
    bool t=false;
    for (size_t i = 0; i < Arr1.getSize(); i++) {
        T3 a;
        a = Arr1[i];
        for (size_t j = 0; j < Arr1.getSize(); j++)
        {
            if (i == j){}
            else {
                T3 b;
                b = Arr1[j];
                if (a == b){
                    t = true;
                    break;}
                else {
                    t = false;
                }
            }
        }
        if (t == false){
            result.push_back(a);
        }
    }
    t = false;
    for (size_t i = 0; i < Arr2.getSize(); i++) {
        T3 a;
        a = Arr2[i];
        for (size_t j = 0; j < result.size(); j++)
        {
            if (i == j) {}
            else {
                T3 b;
                b = result[j];
                if (a == b) {
                    t = true;
                    break;
                }
                else {
                    t = false;
                }
            }
        }
        if (t == false) {
            result.push_back(a);
        }
    }
    MASSIV<T3> connection(result.size());
    for (size_t i = 0; i < result.size(); i++) {
        connection[i] = result[i];
    }
    return connection;
}
template <typename T>
void sameOtdelenie(const MASSIV<T>& people, T t) {
    for (size_t i = 0; i < people.getSize(); i++) {
        if (people[i] == t) {
            cout << people[i] << endl;
        }
    }
}
template <typename T>
void sameOtdelenie2(const MASSIV<T>& people, T t, bool (*compare)(const T&, const T&)) {
    for (size_t i = 0; i < people.getSize(); i++) {
        if (compare(people[i], t)) {
            cout << people[i] << endl;
        }
    }
}
template <typename T1, typename T2>
void read(const string& str, MASSIV<T1>& students1, MASSIV<T2>& students2) {
    ifstream fstruct1(str + "1.txt");
    ifstream fstruct2(str + "2.txt");
    if (fstruct1.is_open()) {
        fstruct1 >> students1;
        fstruct1.close();
    }
    else {
        cout << "Unable to open file: " << str + "1.txt" << endl;
    }
    if (fstruct2.is_open()) {
        fstruct2 >> students2;
        fstruct2.close();
    }
    else {
        cout << "Unable to open file: " << str + "2.txt" << endl;
    }
}
template <typename T3>
void write(const string& str, const MASSIV<T3>& results) {
    ofstream out(str);
    if (out.is_open()) {
        out << "Objedinenie : " << endl;
        out << results;
        out.close();
    }
    else {
        cout << "Unable to open file: " << str << endl;
    }
}
template <typename T>
void sortMassiv(MASSIV<T>& arr, bool (*compare)(const T&, const T&)) {
    size_t size = arr.getSize();
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - 1 - i; j++) {
            if (compare(arr[j + 1], arr[j])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
bool comparekurs(const Element1& a, const Element1& b) {
    return a.kurs<b.kurs;
}
template <typename T>
bool comparator(const T& a, const T& b) {
    return a < b;
}
template <typename T>
bool ravnator(const T& a, const T& b) {
    return a == b;
}
bool speciality(const Element2& a, const Element2& b){
    return a.Otdelenie == b.Otdelenie;
}
int main() {
    MASSIV<Element1> students1(5);
    MASSIV<Element2> students2(4);
  //  read("struct", students1, students2);
  //  students1.writeBinary("students1.bin");
  //  students2.writeBinary("students2.bin");
   students1.readBinary("students1.bin");
   students2.readBinary("students2.bin");
    cout << "students1 : " << endl << students1 << endl;
    cout << "students2 : " << endl << students2 << endl;
    sortMassiv(students1, comparekurs);
    cout << "Sorted students1 : " << endl << students1 << endl;
    cout << "sameOtdelenie : " << endl;
    Element2 t1(103);
    sameOtdelenie(students2, t1);
    cout << endl;
    MASSIV<Element3> students3 = connecter<Element3, Element1, Element2>(students1, students2);
    cout << "Objedinenie : " << endl;
    cout << students3;
    write("active&wellstudying_students.txt", students3);
    cout << endl;

    MASSIV<int> arr1(6);
    MASSIV<int> arr2(6);
    read("arr", arr1, arr2);
    cout << "arr1 : " << endl << arr1 << endl;
    cout << "arr2 : " << endl << arr2 << endl;
    sortMassiv(arr2, comparator);
    cout << "Sorted arr2 : " << endl << arr2 << endl;
    cout << "Same element : " << endl;
    sameOtdelenie(arr2, 32);
    cout <<endl;
    MASSIV<int> arr3 = connecter<int, int, int>(arr1, arr2);
    cout << "arr1 connected arr2 : " << endl;
    cout << arr3;
    write("arr3.txt", arr3);
    cout << endl;

    MASSIV<Element1> students1b(4);
  
    //students1b.readBinary("students1.bin");
    cout << "students1b : " << endl;
    cout << students1b << endl;
    return 0;
}