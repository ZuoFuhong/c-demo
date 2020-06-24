cmake_minimum_required(VERSION 3.16)
project(capp C)

set(CMAKE_C_STANDARD 99)

add_executable(capp
    main.c
    syntax/basic.h
    syntax/basic.c
    syntax/builtin.c
    syntax/pointer.c
    syntax/struct.c
    syntax/union.c
    syntax/file.c
    syntax/syscalls.c
    syntax/bitwise.c
    syntax/process.c
    syntax/error.c
    syntax/signal.c
)
