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
