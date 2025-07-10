#ifndef FLIGHT_H
#define FLIGHT_H

#include <QString>
#include <QVector>

struct Flight {
    qint16 number;
    QString name;
    QString date;
};

void countingSort(QVector<Flight>& flights, bool ascending = true);

#endif
