#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RUS");
	double x, slag{ 1 }, sum{ 0 }, e{ 0 };
	int k { 0 }, tr;
	cout << "������� 'x' " << endl;
	cin >> x;
	cout << "������� �������� ����" << endl;
	cin >> tr;
	e = pow(10, (double)(-tr));
	while (abs(slag) > e)
	{
		sum = sum + slag;
		slag = slag * ((-x) * (k + 1) / (k + 2));
		k = k + 2;
	}
	cout << "����� ���� ����� = " << sum << endl;
	cout << "�������� = " << 1 / (sqrt(1+x)) << endl;
	return 0;
}