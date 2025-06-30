#include <iostream>
#include <string>

using namespace std;

class Abstract {
public:
    Abstract() : ID(count)
    {
        count++;
    }
    virtual ~Abstract()
    {
        count--;
    }
    virtual void display(void) = 0 {};
protected:
    static int count;
    const int ID;
};

int Abstract::count = -1;

class Density : public Abstract {
public:
    Density() : Abstract() {}

    Density(const string& name, double density) : Abstract() {
        this->name = name;
        this->density = density;
    }

    ~Density() {}

    string name;
    double density;
    void display()  override {
        cout << "Название жидкости: " << name << endl;
        cout << "ID ";
        cout << ID << endl;
        cout << "Плотность: " << density << " г/см^3" << endl;
    }
};

class Juice : public Density {
public:
    Juice() : Juice("Unknown", 1.0, 0) {}

    Juice(const string& name, double density, double naturalPercentage) : Density() {
        this->name = name;
        this->density = density;
        this->naturalPercentage = naturalPercentage;
    }

    ~Juice() {};

    void setDensity(double newDensity) {
        density = newDensity;
    }

    void setNaturalPercentage(double newNaturalPercentage) {
        naturalPercentage = newNaturalPercentage;
    }

void display() override {
    Density Name(name, density);
    Name.display();
        cout << "Натуральный процент: " << naturalPercentage << "%" << endl;
        cout << "Всего соков: " << count << endl << endl;
    }

private:
    double naturalPercentage;
};

int main() {
    setlocale(LC_ALL, "RUS");
    Juice appleJuice("Яблочный сок", 1.05, 100);
    appleJuice.display();

    Juice orangeJuice("Апельсиновый сок", 1.03, 90);
    orangeJuice.display();

    appleJuice.setDensity(1.06);
    appleJuice.setNaturalPercentage(95);
    
    cout << "\nПосле обновлений:\n";
    appleJuice.display();

    return 0;
}