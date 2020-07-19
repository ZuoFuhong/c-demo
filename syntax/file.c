// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// 文件和元数据
// 每个文件都由一个inode引用，inode又通过文件系统唯一的inode数值进行寻址。inode既是Unix系统中存放在磁盘中的
// 物理对象，又是Linux内核中由数据结构描述的概念实体。inode 存储文件相关联的元数据，例如文件的访问权限、最后访问
// 时间戳、拥有者、组、和大小，以及文件数据存储位置。
//
// 获取文件元数据，成功时返回0，并存储文件的元数据到stat结构体中。错误时返回-1，并设置errno。
void use_stat() {
    struct stat buf;
    stat("info.log", &buf);
    printf("%lld bytes\n", buf.st_size);
}

// 设置文件的权限 和 拥有者
// 成功时两个调用都返回 0;失败时都返回-1，并设置errno。
void use_chmod() {
    chmod("info.log", 0755);
    // chown("info.log", 0, 0);
}

// 当buf为NULL时，Linux会分配一个size大小的缓冲区，并存储当前工作目录到该缓冲区中。
// 如果size为0，C库会分配一个足够存储当前工作目录的缓冲区。然后由应用在使用完之后，
// 通过 free()释放这个缓冲区。
void use_getcwd() {
    char *cwd = getcwd(NULL, 0);
    if (!cwd) {
        perror("error cause");
        exit(EXIT_FAILURE);
    }
    printf("%s", cwd);
    free(cwd);
}

// 改变当前工作目录
// 成功时调用返回 0; 失败时返回-1，设置 errno。
void use_chdir() {
    chdir("/Users/dazuo/workplace/c-learning-notes");
    use_getcwd();

    char *dir = "/Users/dazuo/workplace/c-learning-notes/demo";
    mkdir(dir, 0755);
    rmdir(dir);
}
