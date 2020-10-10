cmake_minimum_required(VERSION 3.16)
project(capp C)

# 定义变量
set(CMAKE_C_STANDARD 99)
set(INC_DIR /usr/local/include)
set(LINK_DIR /usr/local/lib)

# CMake导入第三方库（1.头文件目录 2.库目录 3.链接的库）
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(zmq hiredis)

# 指定目标文件
add_executable(capp main.c
        third-lib/zeromq/zmq_req_client.c third-lib/zeromq/zmq_rep_server.c)

# 引用第三方库（工程名称 + 库的名称）
target_link_libraries(capp zmq hiredis)
