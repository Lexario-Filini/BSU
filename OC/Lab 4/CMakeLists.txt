cmake_minimum_required (VERSION 3.8)

project ("ProcessSync")

add_compile_definitions(UNICODE _UNICODE)

add_executable (Receiver receiver.cpp common.h)

add_executable (Sender sender.cpp common.h)