//
// Created by dazuo on 2020/6/12.
//
#include <stdio.h>
#include <stdlib.h>

/* 概念：指针是一种保存变量地址的变量 */

void swap(int *px, int *py);
long strlength(char *s);

void learn_pointer() {
    int i = 47;
    int *ap = &i;    // 将 i 的地址赋值给变量 a，称 a 为"指向" i 的指针。地址运算符 & 只能应用于内存中的对象，即变量与数组元素。
    printf("ap = %p\n", ap);

    int b = 20;
    ap = &b;
    printf("ap = %p\n", ap);
    *ap = 21;
    printf("ap = %p\n", ap);
    int c = *ap;
    printf("c = %d\n", c);
    *ap += 2;   // 指向的对象进行运算
    printf("ap = %p\tav = %d\n", ap, *ap);
    ap += 2;    // 地址进行运算
    printf("ap = %p\tav = %d\n", ap, *ap);

    // 每个指针都必须指向某种特定的数据类型。（一个例外情况是指向void类型的指针可以存放指向任何类型的指针，但它不能间接引用其自身。）
    int x = 10;
    int y = 20;
    swap(&x, &y);
    printf("x = %d, y = %d", x, y);
}

void swap(int *px, int *py) {
    int temp = *px;
    *px = *py;
    *py = temp;
}

// 字符数组和指针
void learn_character_pointer() {
    // 下面两个定义有很大区别：
    char amessage[] = "now is the time";     // 定义一个数组
    char *pmessage  = "now is the time";     // 定义一个指针
    // 上述声明中，amessage是一个仅仅足以存放初始化字符串以及空字符'\0'的一维数组。数组中的单个字符可以进行修改，
    // 但amessage始终指向同一个存储位置。另一方面，pmessage是一个指针，其初值指向一个字符串常量，之后它可以被修
    // 改以指向其他地址。但是如果试图修改字符串的内容，结果是没有定义的。

    printf("p = %p, c = %c\n", pmessage, *pmessage);
    printf("p = %p, c = %c\n", pmessage + 1, *pmessage + 1);

    // 修改指针指向
    pmessage = "hello";
    printf("s = %s\n", pmessage);

    // 修改字符串的内容，导致后续代码无法执行
    // *pmessage = 'k';
    // printf("p = %p, c = %c", pmessage, *pmessage);
}

// 数组指针
void learn_array_pointer() {
    int arr[] = {1, 2, 3};
    printf("%p\n", arr);            // 数组名和指针有一个不同之处：指针是变量，但数组名不是变量，因此类似于arr++形式是非法的。
    printf("%p\n", &arr[0]);        // 数组名 指向了数组的第一个元素
    printf("first: %d\n", *arr);
    printf("%d\n", arr[0]);         // 在计算数组元素arr[i]的值时，C语言实际上先将其转换为 *(arr + i) 的形式，然后再求值，因此这两种形式是等价的。
    printf("%d\n", *(arr + 0));

    printf("%p\n", arr + 1);        // 如果arr指向数组中的某个特定元素，那么，更加指针运算的含义，arr + i是数组 arr[i] 的地址 *(arr +i)引用的是数组元素arr[i]的内容。
    printf("%d\n", *(arr + 1));
    printf("%d\n", *(&arr[0] + 1));

    // 当将一个数组名传递给一个函数时，实际上传递的是该数组第一个元素的地址。
    char s[] = "hello";
    long len = strlength(s);
    printf("len = %ld\n", len);
}

// 返回字符串的长度
// long strlen(char s[]) 两种表现形式等效，后者相比前者更直观地表明了该参数是一个指针。
long strlength(char *s) {
    int n;
    for (n = 0; *s != '\0'; s++) {
        n++;
    }
    return n;
}

#define ALLOCSIZE 1000
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

// 指针的运算
void learn_pointer_operation() {
    printf("%ld\n", allocbuf + ALLOCSIZE - allocp);

    // 指针可以和整数进行相加或相减运算
    // 在计算p + n时，n将根据p指向的对象的长度按比例缩放，而p指向的对象的长度则取决于p的声明。
    // 例如，int类型占4个字节的存储空间，对应的n将按4的倍数来计算。
    //
    // 有效的指针运算包括：
    // - 1.相同类型指针之间的赋值运算;
    // - 2.指针同整数之间的加法或减法运算；
    // - 3.指向相同数组中元素的两个指针间的减法或比较运算；
    // - 4.将指针赋值为0或指针与0之间的比较运算。
    // 其它所有形式的指针运算都是非法的。
    printf("%p\n", allocp);
    printf("%p\n", allocp + 1);
}

// 指针数组
// 指针本身也是变量，可以像其他变量一样存储在数组中
char *lineptr[10];
// 表示lineptr是一个具有10个元素的一维数组，其中数组的每个元素是一个指向字符类型对象的指针。
// 也就是说，lineptr[i]是一个字符指针，而*lineptr[i]是改指针指向的第i个文本行的首字符。
void learn_pointer_array() {
    *lineptr = "one";   // 首元素
    printf("p = %p, s = %s\n", lineptr, *lineptr);

    lineptr[0] = "two"; // 首元素
    printf("p = %p, s = %s\n", lineptr[0], lineptr[0]);

    // lineptr首元素的首字符
    printf("c = %c\n", *lineptr[0]);
}

// 多维数组
void invoke_array(int arr[2][2]);
void invoke2_array(int arr[][2]);
void invoke3_array(int (*arr)[2]);
// 以上 三种表示方式等效，这种形式表示参数是一个指针，它指向具有2个整型元素的一维数组。
// 因为方括号[]优先级高于 * 的优先级，所以上述声明中必须使用圆括号。

// 去掉圆括号后，相当于声明了一个数组，该数组有2个元素，其中每一个元素都是一个指向整型
// 对象的指针。一般来说，除数组的第一维（下标）可以不指定大小外，其余各位都必须指定大小。
void invoke4_array(int *arr[2]);

void learn_multi_array() {
    int arr[][2] = {{1, 2}, {3, 4}};
    printf("%d\n", arr[0][0]);
    invoke_array(arr);
    invoke2_array(arr);
    invoke3_array(arr);
}

void invoke_array(int arr[2][2]) {
    printf("p = %p, d = %d\n", arr[0], *arr[0]);
    printf("p = %p, d = %d\n", arr[0] + 1, *(arr[0] + 1));
}

void invoke2_array(int arr[][2]) {
    printf("p = %p, d = %d\n", arr[0], *arr[0]);
    printf("p = %p, d = %d\n", arr[0] + 1, *(arr[0] + 1));
}

void invoke3_array(int (*arr)[2]) {
    printf("p = %p, d = %d\n", arr[0], *arr[0]);
    printf("p = %p, d = %d\n", arr[0] + 1, *(arr[0] + 1));
}

// 指向函数的指针
// 在C语言中，函数本身不是变量，但可以定义指向函数的指针。这种类型的指针可以被赋值、存放
// 在数组中、传递给函数以及函数的返回值等等。

// 指针数组参数的类型为通用指针类型 void *。由于任何类型的指针都可以转换为 void *类型，
// 并且在将它转换回原来的类型不会丢失信息。
int numcmp(char *s1, char *s2);
void invoke_func(char *left, char *right, int (*comp)(char *, char *));

void learn_func_pointer() {
    invoke_func("12", "13", numcmp);
}

void invoke_func(char *left, char *right, int (*comp)(char *, char *)) {
    int res = (*comp)(left, right);
    printf("%d", res);
}

// 按数值顺序比较字符串 s1 和 s2
int numcmp(char *s1, char *s2) {
    double v1, v2;
    v1 = strtod(s1, NULL);
    v2 = strtod(s2, NULL);
    if (v1 < v2) {
        return -1;
    } else if (v1 > v2) {
        return 1;
    } else {
        return 0;
    }
}