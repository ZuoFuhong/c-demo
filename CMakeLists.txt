cmake_minimum_required(VERSION 3.16)
project(c_demo C)

set(CMAKE_C_STANDARD 99)

add_executable(c_demo
        syntax/basic.h syntax/basic.c main.c)
