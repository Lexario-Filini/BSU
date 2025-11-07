cmake_minimum_required(VERSION 3.16)

project(MarkerThreadsWinAPI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0601) 
endif()

add_executable(MarkerThreadsWinAPI
  src/main.cpp
  include/shared.h
  include/marker.h
)

target_include_directories(MarkerThreadsWinAPI PRIVATE include)

if (MSVC)
  target_link_libraries(MarkerThreadsWinAPI PRIVATE kernel32)
endif()

if (MSVC)
  target_compile_options(MarkerThreadsWinAPI PRIVATE /W4 /permissive-)
else()
  target_compile_options(MarkerThreadsWinAPI PRIVATE -Wall -Wextra -pedantic)
endif()
