#include <iostream>
#include <stdlib.h>
#include <algorithm> 
using namespace std;
void SortBubble(long double  a[], long double  size)
{
    int i, j;
    int t;
    for (i = 0; i < size; i++)
       
        for (j = size - 1; j > i; j--)
           
            if (a[j - 1] < a[j])
            {
                t = a[j - 1]; a[j - 1] = a[j]; a[j] = t;
            }

}
void sort_pod(long double* array, long double* count, long double size)
{
    int i, j;
    for (i = 0; i < size; i++)
        count[i] = 0;
    for (i = 0; i < size - 1; i++)
    {
        for (j = i + 1; j < size; j++)
        {
            if (array[i] < array[j])
                count[j]++;
            else
                count[i]++;
        }
    }
}
int Cmp(const void* val1, const void* val2) {
    long double a, b;
    a = *(long double*)val1;
    b = *(long double*)val2;
    if (a > b) return -1;
    if (a < b) return 1;
    if (a == b) return 0;
}
int Comp(const void* val1, const void* val2) {
    long double a, b;
    a = *(long double*)val1;
    b = *(long double*)val2;
    if (a > b) return 1;
    if (a < b) return -1;
    if (a == b) return 0;
}
int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    long double  f;
    int n;
    cout << "Введите количество элементов массива: ";
    cin >> n;
    long double* Arr;
    Arr = new long double[n];
    for (int i = 0; i < n; i++)
    {
        Arr[i] = rand() % 100;
    }
    SortBubble(Arr, n);
    cout << "Массив c сортировкой пузырька:" << endl;
    for (int i = 0; i < n; i++) {
        cout << Arr[i] << " ";
    }
    sort_pod;
    cout << endl << "Массив c сортировкой подсчета:" << endl;
    for (int i = 0; i < n; i++) {
        cout << Arr[i] << " ";
    }
    qsort(Arr, n, sizeof(long double), Cmp);
    cout << endl << "Массив c сортировкой по убыванию через qsort:" << endl;
    for (int i = 0; i < n; i++) {
        cout << Arr[i] << " ";
    }
    sort (Arr, Arr + n);
    cout << endl << "Массив c сортировкой sort:" << endl;
    for (int i = 0; i < n; i++) {
        cout << Arr[i] << " ";
    }
    cout << endl << "Введите что искать" << endl;
    cin >> f;
    sort(Arr, Arr + n);
    long double* s = (long double*)bsearch(&f, Arr, n, sizeof(long double), Comp);
    cout << "Элемент = " << s- Arr + 1 << endl;
    return 0;
}