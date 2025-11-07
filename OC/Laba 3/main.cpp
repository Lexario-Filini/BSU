#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "shared.h"
#include "marker.h"

int readPositiveInt(const char* prompt, int minVal = 1) {
    while (true) {
        std::cout << prompt;
        long long x;
        if (std::cin >> x && x >= minVal && x <= std::numeric_limits<int>::max()) {
            return static_cast<int>(x);
        }
        std::cout << "Invalid input. Please enter an integer >= " << minVal << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void printArray(const SharedState& s) {
    EnterCriticalSection(const_cast<CRITICAL_SECTION*>(&s.ioCs));
    EnterCriticalSection(const_cast<CRITICAL_SECTION*>(&s.arrayCs));
    std::cout << "Array: [";
    for (size_t i = 0; i < s.array.size(); ++i) {
        std::cout << s.array[i];
        if (i + 1 < s.array.size()) std::cout << ' ';
    }
    std::cout << "]\n";
    LeaveCriticalSection(const_cast<CRITICAL_SECTION*>(&s.arrayCs));
    LeaveCriticalSection(const_cast<CRITICAL_SECTION*>(&s.ioCs));
}

DWORD WINAPI MarkerThreadProc(LPVOID lpParam) {
    auto* p = static_cast<MarkerParams*>(lpParam);
    SharedState& s = *p->shared;
    const int id = p->id;

    srand(static_cast<unsigned>(id));
    std::vector<int> owned;

    WaitForSingleObject(s.startEvent, INFINITE);

    size_t n = s.array.size();
    size_t markedCount = 0;

    while (true) {
        int r = rand();
        size_t idx = static_cast<size_t>(r % static_cast<int>(n));

        bool couldMark = false;
        {
            EnterCriticalSection(&s.arrayCs);
            if (s.array[idx] == 0) {
                Sleep(5);
                s.array[idx] = id;
                Sleep(5);
                owned.push_back(static_cast<int>(idx));
                markedCount++;
                couldMark = true;
            }
            LeaveCriticalSection(&s.arrayCs);
        }
        if (couldMark) continue;

        EnterCriticalSection(&s.ioCs);
        std::cout << "Marker " << id
            << " cannot mark. Count=" << markedCount
            << ", blocked at index " << idx << "\n";
        LeaveCriticalSection(&s.ioCs);

        SetEvent(s.blockedEvents[id - 1]);
        WaitForSingleObject(s.controlEvents[id - 1], INFINITE);

        MarkerCommand cmd = getCommand(s, id - 1);
        ResetEvent(s.controlEvents[id - 1]);

        if (cmd == MarkerCommand::Terminate) {
            EnterCriticalSection(&s.arrayCs);
            for (int pos : owned) {
                s.array[static_cast<size_t>(pos)] = 0;
            }
            LeaveCriticalSection(&s.arrayCs);

            EnterCriticalSection(&s.ioCs);
            std::cout << "Marker " << id << " terminated. Cleared " << owned.size() << " positions.\n";
            LeaveCriticalSection(&s.ioCs);

            ResetEvent(s.blockedEvents[id - 1]);
            return 0;
        }
        else if (cmd == MarkerCommand::Continue) {
            ResetEvent(s.blockedEvents[id - 1]);
            continue;
        }
        else {
            ResetEvent(s.blockedEvents[id - 1]);
            continue;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    SharedState s;
    s.initCs();

    const int arraySize = readPositiveInt("Enter array size (>=1): ");
    s.array.assign(static_cast<size_t>(arraySize), 0);

    const int markerCount = readPositiveInt("Enter number of marker threads (>=1): ");

    s.startEvent = createManualResetEvent(FALSE);
    if (!s.startEvent) {
        std::cerr << "Failed to create startEvent\n";
        s.destroyCs();
        return 1;
    }

    s.blockedEvents.resize(markerCount);
    s.controlEvents.resize(markerCount);
    s.commands.resize(markerCount);
    s.threadHandles.resize(markerCount);
    s.alive.resize(markerCount, true);

    for (int i = 0; i < markerCount; ++i) {
        s.blockedEvents[i] = createManualResetEvent(FALSE);
        s.controlEvents[i] = createManualResetEvent(FALSE);
        s.commands[i] = static_cast<int>(MarkerCommand::None); 
        if (!s.blockedEvents[i] || !s.controlEvents[i]) {
            std::cerr << "Failed to create events for marker " << (i + 1) << "\n";
            return 2;
        }
    }

    std::vector<MarkerParams*> params(markerCount, nullptr);
    for (int i = 0; i < markerCount; ++i) {
        params[i] = new MarkerParams{ &s, i + 1 };
        HANDLE h = CreateThread(nullptr, 0, MarkerThreadProc, params[i], 0, nullptr);
        if (!h) {
            std::cerr << "Failed to create marker thread " << (i + 1) << "\n";
            return 3;
        }
        s.threadHandles[i] = h;
    }

    SetEvent(s.startEvent);

    int aliveCount = markerCount;
    while (aliveCount > 0) {
        std::vector<HANDLE> waitList;
        waitList.reserve(aliveCount);
        for (int i = 0; i < markerCount; ++i) {
            if (s.alive[i]) waitList.push_back(s.blockedEvents[i]);
        }

        if (!waitList.empty()) {
            DWORD res = WaitForMultipleObjects(static_cast<DWORD>(waitList.size()), waitList.data(), TRUE, INFINITE);
            if (res >= WAIT_ABANDONED_0 && res < WAIT_ABANDONED_0 + waitList.size()) {
                std::cerr << "Wait abandoned on events.\n";
                break;
            }
        }

        printArray(s);

        int victim = -1;
        while (true) {
            victim = readPositiveInt("Enter marker ID to terminate (1..N): ");
            if (victim >= 1 && victim <= markerCount && s.alive[victim - 1]) break;
            std::cout << "Invalid ID or already terminated. Try again.\n";
        }

        setCommand(s, victim - 1, MarkerCommand::Terminate);

        WaitForSingleObject(s.threadHandles[victim - 1], INFINITE);
        CloseHandle(s.threadHandles[victim - 1]);
        s.threadHandles[victim - 1] = nullptr;
        s.alive[victim - 1] = false;
        aliveCount--;

        printArray(s);

        for (int i = 0; i < markerCount; ++i) {
            if (s.alive[i]) {
                setCommand(s, i, MarkerCommand::Continue);
            }
        }
    }

    for (int i = 0; i < markerCount; ++i) {
        if (s.threadHandles[i]) {
            setCommand(s, i, MarkerCommand::Terminate);
            WaitForSingleObject(s.threadHandles[i], INFINITE);
            CloseHandle(s.threadHandles[i]);
        }
        if (s.blockedEvents[i]) CloseHandle(s.blockedEvents[i]);
        if (s.controlEvents[i]) CloseHandle(s.controlEvents[i]);
        delete params[i];
    }
    if (s.startEvent) CloseHandle(s.startEvent);
    s.destroyCs();

    std::cout << "Main finished. All marker threads terminated.\n";
    return 0;
}
