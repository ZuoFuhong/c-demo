//
// Created by dazuo on 2020/6/11.
//
#include <stdio.h>      // 预处理器是编译过程中执行的第一个步骤。#include指令（用于在编译期间把指定文件的内容包含进当前文件中）
#include <string.h>
#include "basic.h"      // 文件名用引号引起来，则在源文件所在位置查找该文件；如果在该位置没有找到文件，则根据相应的规格查找该文件，这个规则同具体的实现有关。
#include <stdlib.h>

#define CODE "10001"    // #define指令，宏替换，用任意字符序列替代一个标记
#define STEP 30
#define max(A, B)  ((A) > (B) ? (A) : (B))

#define UNKNONW 0
#undef  UNKNONW         // 取消标识符的预定义

int key;
#if STEP == 20
    int key = 20;
#elif STEP == 30
    int key = 30;
#endif

void learn_define() {
    printf("%s\n", CODE);
    printf("%d\n", STEP);
    int x = max(8, 5);
    printf("%d\n", x);
    printf("key = %d\n", key);
}

typedef int length;
typedef char *String;

// typedef声明并没有创建一个新类型，它只是为某个已存在的类型增加一个新的名称而已。
// typedef声明也没有增加任何新的语义：通过这种方式声明的变量与通过普通声明方式声明
// 的变量具有完全相同的属性。
void learn_typedef() {
    length age = 20;
    printf("%d\n", age);

    String name = "Lucy";
    printf("%s\n", name);
}

// 外部的static声明多用于变量，当然，它可用于声明函数。通常情况下，函数名字是全局可以访问的，对整个程序的
// 各个部分而言都是可见。但是，如果把函数声明未static类型，则该函数名出了对函数声明所在的文件可见外，其它
// 文件都无法访问。
//
// static也可用于声明内部变量。不管其所在函数是否被调用，它一直存在，而不像自动变量那样，随着所在函数的调
// 用和退出而存在和消失。static类型的变量会一直占据存储空间。
static int PRI_KEY = 888;
static void learn_static() {
    printf("%d", PRI_KEY);
}

extern int FILE_FD;
extern void learn_stdlibh();
// 在一个源程序的所有源文件中，一个外部变量只能在某个文件中定义一次，而其他文件可以通过extern声明来访问它。
// 外部变量的定义中必须指定数组的长度，但extern声明则不一定要指定数组的长度。
// 外部变量的初始化只能出现在其定义中。
void learn_extern() {
    learn_stdlibh();
    printf("FILE_FD: %d", FILE_FD);
}

void learn_printf() {
    int a = 10;
    long l = 11L;
    char b = 'A';
    float  c = 0.2f;
    double d = 0.4;
    int e = 0123;       // 八进制：以0为前缀,其后由0至7的数字组成
    int f = 0x12;       // 16进制：以0x为前缀，其后有0-9A-F组成
    double g = 0.4;
    char s[] = "hello";
    int *p = &f;

    printf("%d\n", a);      // 按十进制整型数据的实际长度输出
    printf("%ld\n", l);     // 输出长整型数据
    printf("%5d\n", a);     // 指定的输出字段的宽度，左边补空格
    printf("%c\n", b);      // 用来输出一个字符
    printf("%f\n", c);      // 用来输出实数，包括单精度和双精度
    printf("%.2f\n", d);    // 指定小数位，超出的进行四舍五入
    printf("%o\n", e);      // 以八进制整数形式输出
    printf("%x\n", f);      // 以十六进制整数形式输出
    printf("%s\n", s);      // 用来输出字符串
    printf("%g\n", g);      // 以最短的形式表示
    printf("%d\n", *p);
    printf("%p\n", p);      // 指针的地址

    int h = 2147483647;
    unsigned int uh = 2147483648;
    printf("%d\n", h);      // 输出整型
    printf("%u\n", uh);     // 输出无符号整型
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
    //
    // 但是，某些函数（比如：isdigit）在结果未真时可能返回任意的非0值。在if、while、for等语句的测试部分中，
    // "真" 就意味着"非0"，这二者之间没有区别。
    char a = 0;     // 十进制数
    char b = '\0';  // 转义字符
    char c = '0';   // 字符
    printf("%d", a == b);
}

void learn_goto() {
    int a = 10;
    if (a > 9) {
        goto tag;
    }
    printf("123");
    tag:
        printf("occur error");
}

void learn_stderr() {
    // 将fprintf函数产生的诊断信息输出到stderr（标准错误文件）
    fprintf(stderr, "hello");

    // 终止程序的执行，任何调用该程序的进程都可以获取exit的参数值，因此，可以通过另一个将该程序作为子进程的程序
    // 来测试该程序的执行是否成功。按照惯例，返回值0表示一切正常，而非0返回值通常表示出现了异常情况。exit为每个
    // 已打开的输出文件调用fclose函数，以将缓冲区中的所有输出写到对应的文件中。

    // 在主程序main中，语句return expr 等价于exit(expr)。但是使用函数exit有一个优点，它可以从其他函数中调用。
    exit(0);
    printf("world");
}
