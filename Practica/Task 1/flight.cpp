#include "flight.h"
#include <algorithm>

void countingSort(QVector<Flight>& flights, bool ascending) {
    if (flights.isEmpty()) return;

    int min = flights[0].number, max = flights[0].number;
    for (const auto& f : flights) {
        if (f.number < min) min = f.number;
        if (f.number > max) max = f.number;
    }

    QVector<int> count(max - min + 1, 0);
    for (const auto& f : flights)
        count[f.number - min]++;

    QVector<Flight> sorted;
    for (int i = 0; i < count.size(); ++i) {
        for (int j = 0; j < count[i]; ++j) {
            auto it = std::find_if(flights.begin(), flights.end(),
                                   [i, min](const Flight& f){ return f.number == i + min; });
            if (it != flights.end()) sorted.append(*it);
        }
    }

    if (!ascending) std::reverse(sorted.begin(), sorted.end());
    flights = sorted;
}
