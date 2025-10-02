cmake_minimum_required(VERSION 3.14)
project(ThreadsArrayLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build GoogleTest-based unit tests" ON)

set(SRC_FILES
    main.cpp
    array_utils.cpp
    threadHandler.cpp
)

set(HEADER_FILES
    array_utils.h
    threadHandler.h
)

add_library(core_lib STATIC
    array_utils.cpp
    threadHandler.cpp
)
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (MSVC)
    target_compile_definitions(core_lib PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

add_executable(main_app
    main.cpp
)
target_include_directories(main_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main_app PRIVATE core_lib)
if (MSVC)
    target_compile_definitions(main_app PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if (BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_executable(tests
    tests.cpp
  )
  target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(tests PRIVATE core_lib GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(tests)
endif()

set_target_properties(core_lib PROPERTIES FOLDER "libs")
set_target_properties(main_app PROPERTIES FOLDER "apps")
if (BUILD_TESTS)
  set_target_properties(tests PROPERTIES FOLDER "tests")
endif()
