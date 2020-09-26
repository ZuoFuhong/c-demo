cmake_minimum_required(VERSION 3.16)
project(capp C)

set(CMAKE_C_STANDARD 99)
set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(zmq)

add_executable(capp
    main.c
    syntax/basic.h
    syntax/basic.c
    syntax/builtin.c
    syntax/pointer.c
    syntax/struct.c
    syntax/union.c
    syntax/bitwise.c
    syntax/process.c
    syntax/error.c
    syntax/signal.c
    syntax/socket.c
    syntax/syscall_io.c
    syntax/lib_io.c
    syntax/file.c
    syntax/memory.c
    syntax/kqueue.c
    syntax/thread.c
)

target_link_libraries(capp zmq)