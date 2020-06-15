//
// Created by dazuo on 2020/6/12.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

int FILE_FD = 999;

int getRandomNum(int seed) {
    srand(seed);
    int c = rand(); // NOLINT(cert-msc30-c, cert-msc50-cpp)
    return c;
}

void learn_stdlibh() {
    double a = strtod("123", NULL);
    long b = strtol("123", NULL, 10);
    printf("%.2f\n", a);
    printf("%ld\n", b);

    printf("%d\n", getRandomNum(2));
}

// 实现：内置stdlib.h#strtol()
int atoi(const char s[]) {
    int i, n;
    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9';i++){
        n = n * 10 + (s[i] - '0');
    }
    return n;
}

void learn_stream_input() {
    int c;
    // read a character
    while ((c = getchar()) != EOF) {
        // output a character
        putchar(c);
    }
}

void learn_ctypeh() {
    char letter = 'a';
    printf("%c", (char)toupper(letter));

    // 若字符 letter 是数字，则返回一个非0，否则返回0
    printf("%d", isdigit(letter));
}

// 字符串操作函数
void learn_stringh() {
    char s[11] = "hello";
    printf("l = %ld\n", strlen(s));
    printf("%d\n", s[5] == '\0');

    char s2[] = "world";
    // 将s2指向的字符串连接到s指向的字符串的末尾
    strcat(s, s2);
    printf("%d\n", s[5] == '\0');
    printf("%d\n", s[10] == '\0');
}

// 命令执行函数
void learn_command_execute() {
    system("date");
    system("echo 123");
}

// 存储管理函数
// 函数malloc和calloc用于动态地分配存储块。
// 1.函数malloc分配成功时，它返回一个指针，该指针指向n个字节长度的未初始化的存储空间，否则返回NULL。
// 2.函数calloc分配成功时，它返回一个指针，该指针指向的空闲空间足以容纳由n个指定长度的对象组成的数组，
//   否则返回NULL。该存储空间被初始化为0。
//
// free(p)函数释放p指向的存储空间，其中，p是此前通过调用malloc或calloc函数得到的指针。存储空间的
// 释放顺序没有什么限制。但是，如果释放一个不是通过调用malloc或calloc函数得到的指针所指向的存储空间，
// 将是一个很严重的错误。使用已经释放的存储空间同样是错误的。
void learn_alloc() {
    // 根据请求的对象类型，malloc或calloc函数返回的指针满足正确的对齐要求。
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

// 数学函数
void learn_mathh() {
    int a = -1;
    printf("%d", abs(a));

    double b = pow(2, 3);
    printf("%g", b);
}
