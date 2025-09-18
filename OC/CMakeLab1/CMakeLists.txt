cmake_minimum_required(VERSION 3.10)
project(Lab1_ProcessCreation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(Creator)
add_subdirectory(Reporter)
add_subdirectory(Main)


