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
#include <stdlib.h>
#include <errno.h>

#define MAXLINE 100
#define LISTENQ 10
#define ADDR    "127.0.0.1"
#define PORT    9999

void socket_client() {
    // 1.调用socket函数，指定期望的通信协议类型（协议族和套接字类型）。在成功时返回一个小的非负整数值，它与文件描述符类似，我们把它称为套接字描述符（socket descriptor）
    // 其中：
    //  - af：为地址族，也就是 IP 地址类型，常用的有 AF_INET 和 AF_INET6。AF 是“Address Family”的简写，INET是“Inetnet”的简写。AF_INET 表示 IPv4 地址，例如 127.0.0.1；AF_INET6 表示 IPv6 地址，例如 1030::C9B4:FF12:48AA:1A2B。
    //  - type：表示数据传输方式，常用的有 SOCK_STREAM 和 SOCK_DGRAM。
    //  - protocol：表示传输协议，常用的有 IPPROTO_TCP 和 IPPTOTO_UDP，分别表示 TCP 传输协议和 UDP 传输协议。
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("socket error");
        return;
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    // 把IP地址转化为用于网络传输的二进制数值
    inet_pton(AF_INET, ADDR, &servaddr.sin_addr);
    // 2.用connect函数来建立与TCP服务器的连接
    //   sockfd是由socket函数返回的套接字描述符，第二个、第三个参数分别是一个指向套接字地址结构的指针和该结构的大小。
    //   客户端在调用函数connect前不必非得调用bind函数，因为如果需要的话，内核会确定源IP地址，并选择一个临时端口作为源端口。
    //
    //   从内核的角度看，使用指向通用套接字地址结构的指针另有原因：内核必须取调用者的指针，把它类型强制转换为
    //   struct sockaddr * 类型，然后检查其中 sa_family 字段的值来确定这个结构的真实类型。
    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    // 3.读取服务器的应答
    int n;
    char recvline[MAXLINE];
    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if(fputs(recvline, stdout) == EOF) {
            printf("fputs error");
        }
    }
}

static void sig_child(int signo);

void socket_server() {
    int     listenfd, connfd;
    struct  sockaddr_in servaddr;
    char    buff[MAXLINE];

    // 处理僵尸进程
    signal(SIGCHLD, sig_child);

    // 1.创建一个套接字
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    // 2.调用bind函数把一个本地协议地址赋于一个套接字。对于网际协议，协议地址是32位的IPv4地址或128位的
    //   IPv6地址与16位的TCP或UDP端口号的组合。
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    // 3.调用listen函数把该套接字转换成一个监听套接字。（常量 LISTENQ 指定系统内核允许在这个监听描述符上排队的最大客户连接数）
    listen(listenfd, LISTENQ);
    for (;;) {
        // 4.进程在accept调用中被投入睡眠，等待某个客户连接的到达并被内核接受。TCP连接使用所谓的三路握手（three-way handshake）
        // 来建立连接。握手完毕时accept返回，其返回值是一个称为 已连接描述符（connected descriptor）的新描述符。
        // 该描述符用于与新近连接的那个客户通信。accept为每个连接到本服务器的客户返回一个新描述符。
        connfd = accept(listenfd, NULL, NULL);
        printf("connfd: %d\n", connfd);
        // 并发服务器
        if ((fork()) == 0) {
            close(listenfd);

            // 获取客户端信息
            struct sockaddr_in remoteAddr;
            socklen_t len = sizeof(remoteAddr);
            getpeername(connfd, (struct sockaddr *) &remoteAddr, &len);
            printf("ppid = %d, pid = %d, remote port = %d\n", getppid(), getpid(), ntohs(remoteAddr.sin_port));

            time_t ticks = time(NULL);
            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            write(connfd, buff, strlen(buff));

            close(connfd);
            exit(0);
        }

        // 5.调用close关闭与客户端的连接。该调用引发正常的TCP连接终止序列：每个方向发送一个FIN，每个FIN又由各自的对端确认。
        //
        //   每个文件或套接字都有一个引用计数器。引用计数器在文件表项中维护，它是当前打开着的引用该文件或套接字的描述符的个数。
        //   fork返回后，listenfd和connfd描述符在父进程和子进程间共享（也就是被复制），因此与这两个套接字相关联的文件表项
        //   各自的访问计数均为2。这么一来，当父进程关闭connfd时，它只是把相应的引用计数值从2减为1。该套接字真正的清理和资源
        //   释放要等到其引用数值到达0时才发生。这会在稍后子进程也关闭connfd时发生。
        close(connfd);
    }
}

// 处理被中断的系统调用
//
// 慢系统调用（Slow system call）
// 该术语适用于那些可能永远阻塞的系统调用。永远阻塞的系统调用是指调用永远无法返回，多数网络支持函数都属于这一类。如：若没有客户
// 连接到服务器上，那么服务器的accept调用就会一直阻塞。
//
// 适用于慢系统调用的基本规则是：当阻塞于某个慢系统调用的一个进程捕获某个信号且相应信号处理函数返回时，该系统调用可能返回一个
// EINTR错误。有些内核自动重启某些被中断的系统调用。
//
// 参考：https://blog.csdn.net/benkaoya/article/details/17262053

static void sig_child(int signo) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated.\n", pid);
    }
}

// 1.为什么会有小端字节序？
//   答案是，计算机电路先处理低位字节，效率比较高，因为计算都是从低位开始的。所以，计算机的内部处理都是小端字节序。
//   但是，人类还是习惯读写大端字节序。所以，除了计算机的内部处理，其他的场合几乎都是大端字节序，比如网络传输和文件存储。
// 2.小端字节序：将低序字节存储在起始地址；大端字节序：将高序字节存储在起始地址。
// 3.主机字节序：系统所用的字节序称为主机字节序（host byte order）
void host_byte_order() {
    union {
        short s;
        char  c[sizeof(short)];
    } un;
    un.s = 0x0102;

    if (un.c[0] == 1 && un.c[1] == 2) {
        printf("big-endian\n");
    } else if (un.c[0] == 2 && un.c[1] == 1) {
        printf("little-endian\n");
    } else {
        printf("unknown\n");
    }
}

// 地址转换函数（下面两个函数对应IPv4和IPv6都适用）
void learn_inet_pton() {
    char *strptr = "127.0.0.1";
    struct in_addr attrptr;

    // 把IP地址转化为二进制数值
    int res = inet_pton(AF_INET, strptr, &attrptr);
    if (res == 0) {
        printf("invalid addr");
        return;
    } else if (res == -1) {
        perror(NULL);
        return;
    }
    printf("%d\n", attrptr.s_addr);

    // 反向转换（从数值格式转换到表达式格式）
    char strptr2[20];
    inet_ntop(AF_INET, (void *)&attrptr, strptr2, INET_ADDRSTRLEN);
    printf("%s\n", strptr2);
}

// 网络字节顺序与本地字节顺序之间的转换函数
void learn_htons() {
    // 将主机的无符号短整形数转换成网络字节顺序（就是将一个数的高低位互换）
    __uint16_t b = htons(256);
    // 将一个16位数由网络字节顺序转换为主机字节顺序
    __uint16_t p = ntohs(b);
    printf("b = %d, p = %d\n", b, p);

    // 针对32位的字节序
    __uint32_t b2 = htonl(1);
    __uint32_t p2 = ntohl(b2);
    printf("b2 = %d, p2 = %d\n", b2, p2);
}

#pragma clang diagnostic pop