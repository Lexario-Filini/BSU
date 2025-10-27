cmake_minimum_required(VERSION 3.20)

project(IndustrialCppTasks
    VERSION 1.0.0
    DESCRIPTION "Factorials, dedup, recursive list reverse - industrial-style C++"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive- /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(industrial_cpp
    src/main.cpp
    src/factorials.cpp
    src/dedup.cpp
    src/linked_list.cpp
)

target_include_directories(industrial_cpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (MSVC)
    target_compile_definitions(industrial_cpp PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

enable_testing()
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    tests/test_all.cpp
    src/factorials.cpp
    src/dedup.cpp
    src/linked_list.cpp
)

target_include_directories(unit_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(unit_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
