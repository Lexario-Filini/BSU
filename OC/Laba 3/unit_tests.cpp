#include <gtest/gtest.h>
#include "shared.h"

TEST(ArrayTest, Initialization) {
    SharedState s;
    s.initCs();
    s.array.assign(5, 0);
    for (int val : s.array) EXPECT_EQ(val, 0);
    s.destroyCs();
}

TEST(ArrayTest, ModificationUnderLock) {
    SharedState s;
    s.initCs();
    s.array.assign(3, 0);
    EnterCriticalSection(&s.arrayCs);
    s.array[1] = 99;
    LeaveCriticalSection(&s.arrayCs);
    EXPECT_EQ(s.array[1], 99);
    s.destroyCs();
}

TEST(ArrayTest, MultipleWrites) {
    SharedState s;
    s.initCs();
    s.array.assign(4, 0);
    EnterCriticalSection(&s.arrayCs);
    s.array[0] = 1;
    s.array[1] = 2;
    s.array[2] = 3;
    LeaveCriticalSection(&s.arrayCs);
    EXPECT_EQ(s.array[0], 1);
    EXPECT_EQ(s.array[1], 2);
    EXPECT_EQ(s.array[2], 3);
    s.destroyCs();
}

TEST(CommandTest, SetAndGetContinue) {
    SharedState s;
    s.initCs();
    s.commands.resize(1);
    s.controlEvents.resize(1);
    s.controlEvents[0] = createManualResetEvent(FALSE);
    setCommand(s, 0, MarkerCommand::Continue);
    EXPECT_EQ(getCommand(s, 0), MarkerCommand::Continue);
    CloseHandle(s.controlEvents[0]);
    s.destroyCs();
}

TEST(CommandTest, SetAndGetTerminate) {
    SharedState s;
    s.initCs();
    s.commands.resize(1);
    s.controlEvents.resize(1);
    s.controlEvents[0] = createManualResetEvent(FALSE);
    setCommand(s, 0, MarkerCommand::Terminate);
    EXPECT_EQ(getCommand(s, 0), MarkerCommand::Terminate);
    CloseHandle(s.controlEvents[0]);
    s.destroyCs();
}

TEST(CommandTest, MultipleCommandsSequence) {
    SharedState s;
    s.initCs();
    s.commands.resize(1);
    s.controlEvents.resize(1);
    s.controlEvents[0] = createManualResetEvent(FALSE);

    setCommand(s, 0, MarkerCommand::Continue);
    EXPECT_EQ(getCommand(s, 0), MarkerCommand::Continue);

    setCommand(s, 0, MarkerCommand::Terminate);
    EXPECT_EQ(getCommand(s, 0), MarkerCommand::Terminate);

    setCommand(s, 0, MarkerCommand::None);
    EXPECT_EQ(getCommand(s, 0), MarkerCommand::None);

    CloseHandle(s.controlEvents[0]);
    s.destroyCs();
}

TEST(EventTest, CreateAndSignal) {
    HANDLE ev = createManualResetEvent(FALSE);
    ASSERT_NE(ev, nullptr);
    DWORD res = WaitForSingleObject(ev, 0);
    EXPECT_EQ(res, WAIT_TIMEOUT);
    SetEvent(ev);
    res = WaitForSingleObject(ev, 0);
    EXPECT_EQ(res, WAIT_OBJECT_0);
    ResetEvent(ev);
    res = WaitForSingleObject(ev, 0);
    EXPECT_EQ(res, WAIT_TIMEOUT);
    CloseHandle(ev);
}

TEST(EventTest, WaitForMultipleObjects) {
    HANDLE ev1 = createManualResetEvent(FALSE);
    HANDLE ev2 = createManualResetEvent(FALSE);
    SetEvent(ev1);
    SetEvent(ev2);
    HANDLE arr[2] = { ev1, ev2 };
    DWORD res = WaitForMultipleObjects(2, arr, TRUE, 100);
    EXPECT_EQ(res, WAIT_OBJECT_0);
    CloseHandle(ev1);
    CloseHandle(ev2);
}

TEST(MarkerSimulationTest, ClearOwnedIndices) {
    SharedState s;
    s.initCs();
    s.array.assign(5, 0);
    std::vector<int> owned;
    EnterCriticalSection(&s.arrayCs);
    s.array[2] = 1;
    s.array[4] = 1;
    owned.push_back(2);
    owned.push_back(4);
    LeaveCriticalSection(&s.arrayCs);
    EnterCriticalSection(&s.arrayCs);
    for (int pos : owned) s.array[pos] = 0;
    LeaveCriticalSection(&s.arrayCs);
    EXPECT_EQ(s.array[2], 0);
    EXPECT_EQ(s.array[4], 0);
    s.destroyCs();
}

TEST(SharedStateTest, InitVectors) {
    SharedState s;
    s.initCs();
    int n = 3;
    s.array.assign(n, 0);
    s.blockedEvents.resize(n);
    s.controlEvents.resize(n);
    s.commands.resize(n);
    s.threadHandles.resize(n);
    s.alive.resize(n, true);
    EXPECT_EQ(s.array.size(), n);
    EXPECT_EQ(s.blockedEvents.size(), n);
    EXPECT_EQ(s.controlEvents.size(), n);
    EXPECT_EQ(s.commands.size(), n);
    EXPECT_EQ(s.threadHandles.size(), n);
    EXPECT_EQ(s.alive.size(), n);
    s.destroyCs();
}
