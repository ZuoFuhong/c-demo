//
// Created by dazuo on 2020/6/12.
//
#include <stdio.h>

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
