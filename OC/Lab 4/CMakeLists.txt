cmake_minimum_required (VERSION 3.14)

project ("ProcessSync")

add_compile_definitions(UNICODE _UNICODE)


add_executable (Receiver receiver.cpp common.h)
add_executable (Sender sender.cpp common.h)


enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    run_tests 
    tests.cpp 
    common.h
)

target_link_libraries(
    run_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)