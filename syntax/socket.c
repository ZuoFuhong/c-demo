// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAXLINE 100
#define LISTENQ 10
#define ADDR    "127.0.0.1"
#define PORT    9999

void socket_client() {
    // 1.socket函数创建一个网际（AF_INET）字节流（SOCK_STREAM）套接字。该函数返回一个小整数描述符，
    // 以后的所有函数调用（如随后的connect和read）就用该描述符这个套接字。
    // 其中：
    //  - af：为地址族，也就是 IP 地址类型，常用的有 AF_INET 和 AF_INET6。AF 是“Address Family”的简写，INET是“Inetnet”的简写。AF_INET 表示 IPv4 地址，例如 127.0.0.1；AF_INET6 表示 IPv6 地址，例如 1030::C9B4:FF12:48AA:1A2B。
    //  - type：表示数据传输方式，常用的有 SOCK_STREAM 和 SOCK_DGRAM。
    //  - protocol：表示传输协议，常用的有 IPPROTO_TCP 和 IPPTOTO_UDP，分别表示 TCP 传输协议和 UDP 传输协议。
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("socket error");
        return;
    }
    // 2.转换为二进制端口号
    __uint16_t port = htons(PORT);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = port;
    // 3.把ip地址转化为用于网络传输的二进制数值
    inet_pton(AF_INET, ADDR, &servaddr.sin_addr);
    // 4.建立与服务器的连接
    //   connect函数应用于一个TCP套接字时，将与由它的第二个参数指向的套接字地址结构指定的服务器建立一个TCP连接。
    //   该套接字地址结构的长度也必须作为该函数的第三个参数指定。
    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    // 5.读取服务器的应答
    int n;
    char recvline[MAXLINE];
    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if(fputs(recvline, stdout) == EOF) {
            printf("fputs error");
        }
    }
}

void socket_server() {
    int     listenfd, connfd;
    struct  sockaddr_in servaddr;
    char    buff[MAXLINE];
    time_t  ticks;

    // 1.创建一个套接字
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    // 2.调用bind函数将套接字与特定的IP地址和端口绑定起来
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    // 3.调用listen函数把该套接字转换成一个监听套接字。（常量 LISTENQ 指定系统内核允许在这个监听描述符上排队的最大客户连接数）
    listen(listenfd, LISTENQ);
    for (;;) {
        // 4.进程在accept调用中被投入睡眠，等待某个客户连接的到达并被内核接受。TCP连接使用所谓的三路握手（three-way handshake）
        // 来建立连接。握手完毕时accept返回，其返回值是一个称为 已连接描述符（connected descriptor）的新描述符。
        // 该描述符用于与新近连接的那个客户通信。accept为每个连接到本服务器的客户返回一个新描述符。
        connfd = accept(listenfd, NULL, NULL);
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));

        // 5.调用close关闭与客户端的连接。该调用引发正常的TCP连接终止序列：每个方向发送一个FIN，每个FIN又由各自的对端确认。
        close(connfd);
    }
}
