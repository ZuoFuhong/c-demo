//
// Created by dazuo on 2020/6/15.
//
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void syscall_pid();
void syscall_read();
void syscall_open();
void syscall_lseek();

// 对于类Unix系统，unistd.h中所定义的接口通常都是大量针对系统调用的封装，如fork、pipe以及
// 各种I/O原语（read、write、close等等）。
void learn_unistd() {
    syscall_lseek();
}

void syscall_pid() {
    int pid = getpid();
    printf("%d\n", pid);
}

// 低级I/O：输入与输出是通过read和write系统调用实现的。
// 在这两个函数中，第一个参数是文件描述符，第二个参数是程序中存放读或写的数据的字符数组，第三个
// 参数是要传输的字节数。每次调用返回实际传输的字节数。在读文件时，函数的返回值可能会小于请求的
// 字节数。如果返回值为0，则表示已到达文件的结尾；如果返回值为-1，则表示发生了某种错误。在写文
// 件时，返回值是实际写入的字节数。如果返回值与请求写入的字节数不相等，则说明发生了错误。
void syscall_read() {
    char c;
    int val = (read(0, &c, 1) == 1) ? (unsigned char) c : EOF;
    printf("%c", (char)val);
}

void syscall_write() {
    int fd = open("demo.txt", (unsigned) O_RDWR | (unsigned) O_CREAT, 0755);

    char s[] = "hello world";
    int n = write(fd, s, sizeof(s));
    printf("%d", n);
}

// 文件描述符：在UNIX操作系统中，所有的外围设备（包括键盘和显示器）都被看做是文件系统中的文件，
// 因此，所有的输入/输出都要通过读文件或写文件完成。也就说，通过一个单一的接口就可以处理外围设备
// 和程序之间的所有通信。

// 通常情况下，在读或写文件之间，必须先将这个意图通知系统，该过程称为打开文件。如果一切正常，
// 操作系统将向程序返回一个小的非负整数，该整数称为文件描述符。任何时候对文件的输入/输出都是
// 通过文件描述符标识文件，而不是通过文件名标识文件。（文件描述符类似于标准库中的文件指针或
// MS-DOS中的文件句柄。）系统负责维护已打开的所有信息，用户程序只能通过文件描述符引用文件。
void syscall_open() {
    // open与fopen很相似，不同的是，前者返回一个文件描述符，它仅仅只是一个int类型的数值，
    // 而后者返回一个文件指针。如果发生错误，open将返回-1。

    // 使用O_CREAT的时候，必须mode参数
    // int fd = open("demo.txt", O_RDWR, 0);
    // printf("fd = %d", fd);

    // 如果creat成功地创建了文件，它将返回一个文件描述符，否则返回-1。如果此文件已存在，
    // creat将把该文件的长度截断为0，从而丢弃原先已有的内容。
    int fd = creat("demo.txt", 0755);
    printf("fd = %d", fd);

    // 函数unlink将文件name从文件系统中删除，它对应标准库函数remove
    unlink("demo.txt");

    // 一个程序同时打开的文件数是有限的（通常为20）。响应地，如果一个程序需要同时处理许多
    // 文件，那么它必须重用文件描述符。函数close()用来断开文件描述符和已打开文件之间的连
    // 接，并释放此文件描述符，以供其它文件使用。
    close(fd);
}

// 随机访问-lseek
// 输入/输出通常是顺序进行的：每次调用read和write进行读写的位置紧跟在前一次操作的位置之后。
// 系统调用lseek可以在文件中任意移动位置而不实际写任何数据。
void syscall_lseek() {
    int fd = open("demo.txt", O_RDWR);
    char c;
    read(fd, &c, 1);
    printf("%c\n", c);

    write(fd, &c, 1);
    read(fd, &c, 1);
    printf("%c\n", c);

    // lseek(int fd, long offset, long origin)
    // 将文件描述符为fd的文件的当前位置设置为offset，其中，offset是相对于origin指定的
    // 位置而言的。随后进行的读写将从此位置开始。
    lseek(fd, 0, 0);
    read(fd, &c, 1);
    printf("%c\n", c);
}














