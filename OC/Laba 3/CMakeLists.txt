cmake_minimum_required(VERSION 3.16)
project(MarkerThreadsWinAPI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MarkerThreadsWinAPI
  src/main.cpp
  include/shared.h
  include/marker.h
)

target_include_directories(MarkerThreadsWinAPI PRIVATE include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(UnitTests
  tests/unit_tests.cpp
)


target_include_directories(UnitTests PRIVATE include)
target_link_libraries(UnitTests gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(UnitTests)
