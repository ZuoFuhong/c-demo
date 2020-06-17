cmake_minimum_required(VERSION 3.16)
project(c_demo C)

set(CMAKE_C_STANDARD 99)

add_executable(c_demo
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
#    example/calc/calc.h
#    example/calc/calc.c
#    example/calc/getop.c
#    example/calc/stack.c
)
