//
// Created by dazuo on 2020/6/11.
//
#include <stdio.h>
#include <string.h>
#include "basic.h"

#define CODE "10001"
#define STEP 20

void learn_printf() {
    int a = 10;
    char b = 'A';
    float  c = 0.2f;
    double d = 0.4;
    int e = 0123;       // 八进制：以0为前缀,其后由0至7的数字组成
    int f = 0x12;       // 16进制：以0x为前缀，其后有0-9A-F组成
    char s[] = "hello";

    printf("%d\n", a);  // 按十进制整型数据的实际长度输出
    printf("%ld\n", sizeof(a));  // 输出长整型数据
    printf("%5d\n", a); // 指定的输出字段的宽度，左边补空格
    printf("%u\n", a);  // 输出无符号整型
    printf("%c\n", b);  // 用来输出一个字符
    printf("%f\n", c);  // 用来输出实数，包括单精度和双精度
    printf("%.2f\n", d);// 指定小数位，超出的进行四舍五入
    printf("%o", e);    // 以八进制整数形式输出
    printf("%x", f);    // 以十六进制整数形式输出
    printf("%s", s);    // 用来输出字符串
}

void learn_data_type() {
    char a = 'a';
    char a2 = 127;  // -128 - 127
    unsigned char a3 = 128; // 0 ~ 255
    printf("a = %d\n", a);
    printf("a size = %ld\n", sizeof(a3));

    int b = 10;
    short int c = 20;
    long int d = 10L;
    unsigned long d2 = 100UL;   // UL 表示是 unsigned long类型（注意：这只是语义上表示）
    printf("d = %ld\n", d);
    printf("d2 = %ld\n", d2);

    float e = 2.2f;
    double g = 2.3;
    printf("%.2f", g / 3);
    printf("\n");
    printf("%x\n", 0x12);

    char str[] = "hello中国";
    printf("len = %ld\n", strlen(str)); // 返回字符串参数str的长度，长度不包含末尾的 \0
    printf("len2 = %ld", sizeof(str));  // 返回在内存中占用的字节数，长度包含末尾的 \0
}

void learn_flow_statement() {
    for (int i = 0; i < 5; i++) {
        printf("i = %d\t", i);
    }
}

void learn_define() {
    printf("%s", CODE);
    printf("%d", STEP);
}

void learn_stream_input() {
    int c;
    // read a character
    while ((c = getchar()) != EOF) {
        // output a character
        putchar(c);
    }
}

void learn_array() {
    int list[8];
    // list[1] = 0; TODO: 第二个默认值不是0
    for (int i = 0; i < 8; ++i) {
        printf("%d", list[i]);
    }
}

void learn_char_array() {
    char a = 0;
    char b = '\0';  // a == b true
    /*
        概念：当在C语言程序中出现类似于 "hello" 的字符串常量时，它将以字符数组的形式存储，数组的各元素分别存储字符串的各个字符，并以 '\0' 标志字符串的结束。

        字符串在内存中的实际存放情况，后面的'\0'是由C编译系统自动加上的。
        ——————————————————————————
        | h | e | l | l | o | \0 |
        ——————————————————————————
    */
    char s2[] = "hello";
    // sizeof 可以获得数据类型或变量在内存中所占的字节数
    printf("s2 = %s\t size = %ld\n", s2, sizeof(s2));  // size = 6
}

void learn_union() {
    union data {
        int age;
        char *name;
    } a;
    a.name = "dazuo";
    printf("name = %s\n", a.name);

    a.age = 20;
    printf("%d\n", a.age);
}

void learn_enum() {
    // 枚举是一个常量整型值的列表。
    // 在没有显示说明的情况下，enum类型中第一个枚举名的值为0，第二个为1，以此类推。如果只指定了部分枚举名的值，
    // 那么未指定值的枚举名的值将依着最后一个指定值向后递增。
    // 不同枚举的名字必须互不相同，同一枚举中不同的名字可以具有相同的值。
    enum Week {
        Mon = 1, Thes = 3, Wed
    };
    enum Week day = Mon;
    printf("%d, sizeof = %ld\n", Wed, sizeof(Wed));
    printf("day = %d", day);
}

void learn_const() {
    int a;  // 未经显示初始化的自动变量的值为未定义值（即无效值）
    const int c = 10;
    const char s[] = "hello";   // const限定符指定数组所有元素的值都不能被修改
}

void learn_bool() {
    // 根据定义，在关系表达式或逻辑表达中，如果关系为真，则表达式的结果值为数值1；如果为假，则结果值为数值0。
    // 逻辑非运算符 ! 的作用是将非0操作数转换为0，将操作数0转换为1。
    char a = 0;
    char b = '\0';
    printf("%d", a != b);
}

void learn_pointer() {
    int i = 47;
    int *a = &i;
    int b = 20;
    a = &b;
    *a = 21;
    int c = *a;
    a += 2;
    // array pointer
    int arr[] = {1, 2, 3};
    printf("%p\n", arr);
    printf("%p\n", arr + 1);
    printf("%p\n", &arr[0]);
    printf("%p\n", &arr[0] + 1);
    // array first address offset sizeof(int) unit
    printf("%d\n", *(arr + 1));

    printf("%p\n", &arr);
    // array first address offset sizeof(arr) unit
    printf("%p\n", &arr + 1);
}

void learn_struct() {
    struct stru {
        char *name;
        int age;
    } stru1;

    stru1.name = "dazuo";
    stru1.age = 30;

    printf("name = %s , age = %d\n", stru1.name, stru1.age);

    struct stru *pstru = &stru1;
    printf("%s\n", pstru->name);
}

void learn_file() {
    char file_name[] = "demo.txt";
    FILE *fp = fopen(file_name, "r+");
    if (fp == NULL) {
        fp = fopen(file_name, "w");
    }

    fputs("hello world", fp);

    fclose(fp);
}
