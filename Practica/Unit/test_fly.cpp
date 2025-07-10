#include <QtTest/QtTest>
#include "D:/C++/Praktica/Praktika1/flight.h"

class TestFly : public QObject {
    Q_OBJECT

private slots:
    void test_sortAscending();
    void test_sortDescending();
    void test_stabilityInSort();
};

void TestFly::test_sortAscending() {
    QVector<Flight> flights = {
        {45, "Belavia", "2025-07-09"},
        {12, "Turkish", "2025-07-08"},
        {78, "Lufthansa", "2025-07-10"}
    };

    countingSort(flights, true);

    QCOMPARE(flights[0].number, 12);
    QCOMPARE(flights[1].number, 45);
    QCOMPARE(flights[2].number, 78);
}

void TestFly::test_sortDescending() {
    QVector<Flight> flights = {
        {13, "Belavia", "2025-07-09"},
        {42, "Turkish", "2025-07-08"},
        {7,  "Lufthansa", "2025-07-10"}
    };

    countingSort(flights, false);

    QCOMPARE(flights[0].number, 42);
    QCOMPARE(flights[1].number, 13);
    QCOMPARE(flights[2].number, 7);
}

void TestFly::test_stabilityInSort() {
    QVector<Flight> flights = {
        {10, "A", "2025-07-01"},
        {12, "B", "2025-07-02"}
    };

    countingSort(flights, true);

    QCOMPARE(flights[0].name, "A");
    QCOMPARE(flights[1].name, "B");
}

QTEST_MAIN(TestFly)
#include "test_fly.moc"
