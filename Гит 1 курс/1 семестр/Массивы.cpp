#include <iostream>
#include <ctime> 
using namespace std;
int main ()
{
    const int n = 10;
    int arr[n];
    int s = 0; int k = 0;
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 1000;
        i++;
        }
    for (int i = 0; i < n; i++)
    {
        k = arr[i];
        while (k > 0) {
            s = s * 10 + k % 10;
            k /= 10;
        }
        arr[i+1] = s;
        s = 0;
        i++;
    }
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    return 0;
}