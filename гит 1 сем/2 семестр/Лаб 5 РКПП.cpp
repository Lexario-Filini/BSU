#include <QCoreApplication>
#include <QList>
#include <QVector>
#include <QDebug>
#include <QTextStream>
#include <algorithm>
#include <QtAlgorithms>
#include <QBitArray>
#include <bitset>
#include <iostream>
#include <iterator>
#include <QFile>

using namespace std;
void printList(const QList<int>& lst) {
    QStringList result;
    for (int num : lst ) {
        result.append(QString :: number(num));
}
qDebug() << "Spisok:" << result.join(" ");
}

void deleteElements(QList<int>& lst, int position, int count) {
    if (position >=0 && position < lst.size()) {
        lst.erase(lst.begin() + position, lst.begin() + qMin(position + count, lst.size()));
    }
}

void insertElements(QList<int>& lst, int position, const QVector<int>& elements) {
    if ( position >= 0 && position <= lst.size()) {
        for ( int num : elements) {
            lst.insert(position++, num);
        }
    }
}

bool searchSubsequence(const QList<int>& lst, const QVector<int>& pattern) {
    QVector<int> squares;
    transform(pattern.begin(), pattern.end(), back_inserter(squares), [](int n) { return n*n;});
    return search(lst.begin(), lst.end(), squares.begin(), squares.end()) != lst.end();
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QFile file("input.txt");

    QList<int> myList = { 1, 4, 9, 36, 16, 25, 49, 64, 81, 100 };

    QTextStream in(&file);
    QVector<int> numbers;

    while(!in.atEnd()) {
        QString line = in.readLine();
        for (const QString &numStr : line.split(" ", Qt::SkipEmptyParts)) {
            numbers.append(numStr.toInt());
        }
    }
    file.close();

    //QVector<int> array = { 2, 3, 4 };
    int choice, pos, count;


    do{
    qDebug() << "Menu:";
    qDebug() << "1. Prosmotret sostoyanie spiska";
    qDebug() << "2. Udalit neskolko elementov s zadannoy pozizzii";
    qDebug() << "3. Dobavit neskolko elementov v zadannuu posiziu s massiva";
    qDebug() << "4. Dobavit neskolko elementov iz massiva";
    qDebug() << "5. Nayti v massive zelyh chisel posled, kagdyy element kotoroy raven kvadratu drugogo massiva";
    qDebug() << "6. Poisk naibolchego otsortirovannogo diapazona";
    qDebug() << "7. baytovye preabrozavaniya";
    qDebug() << "8. sravnenie spiskov";
    qDebug() << "9. Vyhod iz programmy";
    qDebug() << "Vvedite vash vybor: ";
    cin >> choice;

    switch (choice) {
    case 1:
        printList(myList);
        break;
    case 2:
        qDebug() << "Vvedite pozizziu dlya udaleniya: ";
        cin >> pos;
        qDebug() << "Vvedite kolichestvo elementov dlya udaleniya: ";
        cin >> count;
        deleteElements(myList, pos, count);
        break;
    case 3:
        qDebug() << "Vvedite pozizziu dlya vstavki: ";
        cin>> pos;
        insertElements(myList, pos, numbers);
        break;
    case 4:
        myList.append(numbers.toList());
        break;
    case 5:
        if (searchSubsequence(myList, numbers)) {
            qDebug() << "Podposledovatelnost naydena.\n";
        }
        else {
            qDebug() << "Podposledovatelnost ne naydena.\n";
        }
        break;
    case 6:{
        auto maxSegmentBegin = myList.begin();
        auto maxSegmentEnd = myList.begin();
        int maxLength = 0;
        auto it = myList.begin();
        while (it != myList.end()) {
            auto segmentEnd = is_sorted_until(it, myList.end());
            int currentLength = distance(it, segmentEnd);
            if (currentLength > maxLength) {
                maxLength = currentLength;
                maxSegmentBegin = it;
                maxSegmentEnd = segmentEnd;
            }
            if (segmentEnd == myList.end())
                break;
            it = segmentEnd;
        }

        qDebug() << "Naibolshyi otsortirovannyi diapazon massiva: ";
        for (auto iter = maxSegmentBegin; iter != maxSegmentEnd; ++iter) {
            qDebug() << *iter << " ";
        }
        break;
    }
    case 7:
    {
        QList<quint8> list8;
        qDebug() << "list befor preobrazovaniy : " << "\n";
        for (int i = 0; i < myList.size(); i++) {
            if (myList[i]>0 && myList[i]<256) {
                list8.push_back(myList[i]);
              }
            qDebug() << myList[i] << "\n";
        }
        qDebug() << "list after preobrazovaniy : " << "\n";
        for (int i = 0; i < list8.size(); i++) {
            cout << "element : "<< (int)list8[i] << " amount of trailing zero bits : "
                 << qCountTrailingZeroBits(list8[i]) << " for checking : " << bitset<8>(list8[i])<<"\n";
        }
        break;
    }
    case 8:{
        QList<int> listA = {1, 3, 5, 7};
        QList<int> listB = {2, 4, 6, 8};
        QList<int> listC = {1, 3, 5, 7};
        qDebug() << "list A = list B?" << (equal(listA.begin(), listA.end(), listB.begin())? "Yes" : "No");
        qDebug() << "list A = list C?" << (equal(listA.begin(), listA.end(), listC.begin())? "Yes" : "No");
        break;
    }
    case 9:
        return 0;
    default:
        qDebug() << "Nevernyy vybor. Poprobuyte eche raz.\n";
        break;
    }
    }while(choice!=7);
    return 0;
}
