#pragma once
#include <vector>
#include <windows.h>

enum class MarkerCommand : int {
    None = 0,
    Continue = 1,
    Terminate = 2
};

struct SharedState {
    std::vector<int> array;
    CRITICAL_SECTION arrayCs;

    HANDLE startEvent = nullptr; 

    std::vector<HANDLE> blockedEvents;   
    std::vector<HANDLE> controlEvents;   
    std::vector<int> commands;           

    std::vector<HANDLE> threadHandles;
    std::vector<bool> alive;

    CRITICAL_SECTION ioCs;

    void initCs() {
        InitializeCriticalSection(&arrayCs);
        InitializeCriticalSection(&ioCs);
    }

    void destroyCs() {
        DeleteCriticalSection(&arrayCs);
        DeleteCriticalSection(&ioCs);
    }
};

struct MarkerParams {
    SharedState* shared;
    int id; 
};

inline HANDLE createManualResetEvent(BOOL initialState = FALSE) {
    HANDLE h = CreateEventW(nullptr, TRUE, initialState, nullptr);
    return h;
}

inline void setCommand(SharedState& s, int idx, MarkerCommand cmd) {
    EnterCriticalSection(&s.arrayCs); 
    s.commands[idx] = static_cast<int>(cmd);
    LeaveCriticalSection(&s.arrayCs);
    SetEvent(s.controlEvents[idx]); 
}

inline MarkerCommand getCommand(SharedState& s, int idx) {
    EnterCriticalSection(&s.arrayCs);
    int val = s.commands[idx];
    LeaveCriticalSection(&s.arrayCs);
    return static_cast<MarkerCommand>(val);
}
