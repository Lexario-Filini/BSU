#pragma once
#include <cstdint>
#include <cstring>

#pragma pack(push,1)
struct employee {
    int32_t num;
    char name[10];
    double hours;
    employee() {
        num = 0;
        std::memset(name, 0, sizeof(name));
        hours = 0.0;
    }
};
#pragma pack(pop)

enum Command : int32_t {
    READ_REQUEST = 1,
    WRITE_REQUEST = 2,
    WRITE_SUBMIT = 3,
    RELEASE = 4,
    CLIENT_EXIT = 5
};
