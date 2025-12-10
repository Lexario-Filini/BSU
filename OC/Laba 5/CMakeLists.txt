cmake_minimum_required(VERSION 3.15)
project(EmployeeIPC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(server server.cpp employee.h)
add_executable(client client.cpp employee.h)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests.cpp employee.h)
target_link_libraries(tests gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
