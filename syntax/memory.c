// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.
#include <stdlib.h>
#include <stdio.h>
#include <alloca.h>
#include <strings.h>
#include <string.h>

// 内存管理（分配、操作、释放）
//
// 函数malloc和calloc用于动态地分配存储块。
// 1.函数malloc分配成功时，它返回一个指针，该指针指向n个字节长度的未初始化的存储空间，失败时返回NULL，
//   并设置 errno 为 ENOMEM。
// 2.函数calloc分配成功时，它返回一个指针，该指针指向的空闲空间足以容纳由n个指定长度的对象组成的数组，
//   该存储空间被初始化为0。失败时返回NULL，并设置 errno 为 ENOMEM。
//
// free(p)函数释放p指向的存储空间，其中，p是此前通过调用malloc或calloc函数得到的指针。存储空间的
// 释放顺序没有什么限制。但是，如果释放一个不是通过调用malloc或calloc函数得到的指针所指向的存储空间，
// 将是一个很严重的错误。使用已经释放的存储空间同样是错误的。
void learn_alloc() {
    char *mp = malloc(2048);
    if (!mp) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    free(mp);

    int *ip = (int *)calloc(4, sizeof(int));
    *ip = 20;
    printf("%p\t%d\n", ip, *ip);
    free(ip);

    // 空悬指针（dangling pointer）：指向已经销毁的对象或已经回收的地址。当free调用时，除了释放
    // 动态申请的内存，还要将相关的指针指向NULL，避免出现空悬指针。
    ip = NULL;

    // 野指针：没有初始化的指针就是野指针。
    // “野指针”可能指向任意内存段，因此它可能会损坏正常的数据，也有可能引发其他未知错误。
    char *p;
    // 避免"野指针"出现，赋初值。
    char *p2 = NULL;
}

struct Member {
    char *name;
    int  age;
};

// 调整分配大小
// void * realloc (void *ptr, size_t size);
// 成功调用realloc()调整ptr指向的内存区域的大小为size。它返回新调整后的内存指针，这个指针可能和ptr一样，
// 也可能不一样——当扩大一个内存区域时，如果realloc()不能在当前位置扩大内存块，函数可能会分配一个新的size字节的
// 内存区域，把原来的区域复制到新内存区域中，并释放旧的内存区域。不管是增大还是缩小，其中的一部分内存区域是保持不变的。
void learn_realloc() {
    printf("%ld\n", sizeof(struct Member));

    struct Member *p = calloc(2, sizeof(struct Member));
    if (!p) {
        perror("alloc");
        exit(EXIT_FAILURE);
    }
    p[0].name = "dazuo";
    p[0].age  = 24;
    p[1].name = "Lucy";
    printf("%p, %s\n", p, p->name);
    printf("%p, %s\n", p + 1, (p + 1)->name);

    struct Member *r = realloc(p, sizeof(struct Member));
    if (!r) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", r[0].name);
    printf("%s\n", r[1].name);

    // 自动分配在堆栈展开时会自动回收内存，动态分配则在进程地址空间中永久存在，直到它们被手动释放。
    free(r);
}

// 多数情况下，编译器和C库能够透明地处理对齐问题。POSIX规则malloc()、calloc()、和 realloc()返回的内存对所有标准类型都正确对齐。
// 在Linux中，32位系统总是返回8字节边界对齐的内存; 64位系统总是返回16字节对齐的内存。
void learn_memalign() {
}

// 基于堆栈的分配
// 在栈中分配动态内存，可以使用 alloca()系统调用成功时alloca()返回一个指向size字节大小的内存指针。这段内存存在于堆栈中，
// 并且在调用函数返回时自动释放。有一些实现在失败时返回 NULL，但是大多数alloca()实现都不会失败，或者无法报告失败。失败就表示堆栈已经溢出。
void learn_alloca() {
    char *dir = "/Users/dazuo";
    char *dir2 = alloca(strlen(dir) + 1);
    strcpy(dir2, dir);

    printf("%s", dir2);
}

// void * memset (void *s, int c, size_t n);
// 调用memset()设置指针s开始的n个字节为字节c，并返回 s。常见的用法是对一块内存清零。
void learn_memset() {
    char s[] = "hello world";
    memset(s, '\0', sizeof(s));

    printf("%c\n", s[0]);
    printf("%d\n", s[0] == '\0');
}

// bzero 会将参数s 所指的内存区域前n个字节，全部设为零值。
// bzero()是一个由BSD引入的更老的，已经被不赞成使用的接口，完成相同的任务。新的代码应该使用memset()。
void learn_bzero() {
    char s[] = "hello";
    // 将一个数组清零
    bzero(s, sizeof(s));
    printf("%s, %ld\n", s, sizeof(s));

    struct user {
        char *name;
        int  age;
    } admin;
    // 将一个结构体清零
    bzero(&admin, sizeof(admin));
    printf("%s, %d, %ld\n", admin.name, admin.age, sizeof(admin));
}

void learn_memory() {
    learn_memset();
}
