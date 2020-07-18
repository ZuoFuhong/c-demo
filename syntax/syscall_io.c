//
// Created by dazuo on 2020/6/15.
//
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/*
    文件I/O 系统调用
*/

// 在文件被读取和写入之前，它必须先被打开。内核为每个进程保存了一个打开文件的列表，被称为文件表（file table）。文件表
// 通过一个非负整数来索引，也就是文件描述符（通常简称为fd）。列表中的每一个条目都包含一个打开文件的信息，包括一个指针，
// 指向文件 inode 在内存中的一份拷贝，和相应的元数据，例如文件位置和访问模式。用户空间和内核空间的每个进程使用的文件描述
// 符都是独立的。打开文件返回一个文件描述符，随后的操作（读取、写入等等）都使用文件描述符作为主要的参数。

// 默认情况下，子进程将复制父进程的文件表。打开的文件列表、访问模式、当前文件位置等等都是一样的，但子进程对文件表的更改
// （例如子进程关闭文件）并不会影响其它进程。

// Linux文件描述符使用C语言int类型描述，而没有使用类似于fd_t这样的特殊类型。每个Linux都有一个允许打开文件的最大数量值，
// 文件描述符从0开始，增长到比最大数量值小1。默认情况下，这个最大值是1024。
// 每个进程通常都至少有三个已打开的文件描述符：0，1和2，除非进程显示地关闭它们。文件描述符 0 是标准输入（stdin），文件
// 描述符 1 是标准输出（stdout），文件描述符 2 是标准错误（stderr）。
void sys_fd() {
    printf("%d - %d - %d", STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
}

// open()在成功时都返回一个文件描述符，出错时，两个函数都返回-1，而且会设置errno为相应的错误值。
// 语法：int open (const char *name, int flags, mode_t mode);
//  - flag：O_RDONLY（只读）、O_WRONLY（只写）、O_RDWR（读写）
//  - mode：在创建文件时，mode参数提供新创建文件的权限。除非创建了新文件，mode参数都会被直接忽略。
//
// 当指定了O_CREAT标记时，mode参数就是必需的。如果使用O_CREAT时忘记了提供mode参数，结果将是未定义的，
// 而且通常都很糟糕——所以千万别忘记了。
void syscall_open() {
    int fd = open("info.log", O_RDWR);
    if (fd == -1) {
        perror("error cause");
    } else {
        printf("fd = %d", fd);
    }

    int fd2 = open("info.txt", O_CREAT, 0755);
    if (fd2 == -1) {
        perror("error cause");
    } else {
        printf("fd2 = %d", fd);
    }
}

// 如果creat成功地创建了文件，它将返回一个文件描述符，否则返回-1。如果此文件已存在，
// creat将把该文件的长度截断为0，从而丢弃原先已有的内容。
void syscall_creat() {
    int fd = creat("demo.txt", 0755);
    if (fd == -1) {
        perror("error cause");
    } else {
        printf("fd = %d", fd);
    }
}

// 每次调用 read()将从 fd 指向文件的当前位置开始，读取最多 len个字节到缓冲区buf中。
// 读取成功时，函数返回写入到buf中的字节数。出现错误时，调用将返回-1，同时设置errno。
// 文件位置根据已读取的字节数更新。
//
// 语法：ssize_t read (int fd, void *buf, size_t len);
//
// 使用 read()时另一个需要考虑的是函数可能返回 0，read()系统调用返回0表示已到文件尾
// end-of-file(EOF)，注意区分 define EOF (-1)
void syscall_read() {
    char c;
    int len = 1;
    if (read(STDIN_FILENO, &c, len) == EOF) {
        perror("error cause");
    } else {
        printf("%d, %c", len, c);
    }
}

// 调用write()把buf指向的 count个字节数据写入到文件描述符fd指向文件的当前文件位置。
// 成功时返回写入的字节数，同时更新文件位置，出错时返回 -1，并设置errno为相应的错误代码。
// write()调用可以返回0，但这个返回值没有任何特殊的意义，它只是简单地表示0字节数据被写入。
void syscall_write() {
    char s[] = "hello world";
    int n = write(STDOUT_FILENO, s, sizeof(s));
    printf("\n%d", n);
}

// fsync()系统调用确保文件描述符fd指向的文件相关联的所有数据都被写入磁盘。文件描述符fd
// 必须以写入模式打开。这个调用同时写入数据和元数据，例如创建时间戳、以及其它包含在inode
// 中的属性。只有当磁盘驱动器显示数据和元数据全部写到磁盘后，fsync()调用才会返回。
//
// fdatasync()系统调用和fsync()做相同的事情，不过它只刷新数据。fdatasync()不保证元
// 数据也被同步到磁盘中，因此可能会比fsync更快。
void syscall_fsync() {
    int fd = open("demo.txt", O_RDWR);
    if (fd == -1) {
        perror("error cause");
    } else {
        char s[] = "hello world";
        write(fd, s, sizeof(s));
        fsync(fd);
        close(fd);
    }
}

// 调用 close()解除打开的文件描述符fd的映射，并且把进程与文件分离。给定的文件描述符随后
// 不再合法，在接下来的open()或creat()调用中，内核可以自由地重用并返回它。close()调用
// 成功时返回 0;错误时返回-1，并适当地设置 errno。
//
// 注意：关闭文件并不会使文件数据写入到磁盘。如果应用程序希望确保文件被提交到磁盘之后再关闭，
// 则需要使用fsync()确保数据都写入磁盘。

// 当与文件相关联的最后一个已打开文件描述符被关闭时，内核中描述该文件的数据结构将被释放。
// 当这个数据结构被释放时，它将拔去文件相关联的 inode 在内存中的拷贝。
void syscall_close() {
    int fd = open("info.log", O_RDWR);
    if (fd == -1) {
        perror("error cause");
    } else {
        printf("fd = %d", fd);
        close(fd);
    }
}

// lseek()系统调用设置指定的文件描述符的文件位置为一个给定的值。除了更新文件位置，不执行
// 任何其它动作，也不会发送任何I/O请求。返回更新后的文件位置，出错时返回-1，并设置errno。
//
// 语法：off_t lseek (int fd, off_t pos, int origin)
// 将文件描述符为fd的文件的当前位置设置为offset，其中，offset是相对于origin指定的
// 位置而言的。随后进行的读写将从此位置开始。
void syscall_lseek() {
    int fd = open("demo.txt", O_RDWR);
    char c;
    read(fd, &c, 1);
    printf("%c\n", c);

    lseek(fd, 0, 0);
    read(fd, &c, 1);
    printf("%c\n", c);
}

// Linux提供read()和 write()系统调用的两个变体，它们分别接受一个文件位置参数并从那里
// 读取或者写入。操作完成后，它们并不更新文件位置。
//
// 成功时，两个调用都返回读取或者写入的字节数，pread()返回 0 表示到达 EOF；pwrite()
// 返回 0 则表示调用并没有写入任何东西。错误时，两个调用均返回-1，并设置errno。
void syscall_pread() {
    int fd = open("demo.txt", O_RDWR);
    char c;
    pread(fd, &c, 1, 1);
    printf("%c", c);

    char s[] = "ok";
    pwrite(fd, s, sizeof(s), 1);
}

// int ftruncate (int fd, off_t len);
// int truncate (const char *path, off_t len);
//
// 这两个系统调用将指定的文件的长度截短为len。ftruncate()系统调用对fd指定的文件进行
// 操作，必须以写入方式打开。truncate()系统调用则对路径名 path 指定的文件进行操作，
// 文件必须具有写入权限。二者成功时均返回 0;出错时返回-1，并设置 errno 值。

// 这两个系统调用最常见的用途是把一个文件截短为比当前长度更小。当调用成功返回时，文件的
// 新大小将是len。原先那些在 len和文件末尾之间的数据被丢弃，读取请求不能再访问它们。
void syscall_truncate() {
    int fd = open("demo.txt", O_RDWR);
    int len = ftruncate(fd, 5);
    if (len == -1) {
        perror("error cause");
    }
}
