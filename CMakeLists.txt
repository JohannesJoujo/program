cmake_minimum_required(VERSION 3.24)
project(program)

set(CMAKE_CXX_STANDARD 17)

add_executable(program main.cpp lexer.cpp lexer.h expr.cpp expr.h)
