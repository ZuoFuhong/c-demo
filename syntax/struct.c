//
// Created by dazuo on 2020/6/14.
//
#include <stdio.h>

struct stru {
    char *name;
    int age;
};

// 结构是一个或多个变量的集合。
void learn_struct() {
    struct stru stru1 = {"mi", 20};
    stru1.name = "dazuo";
    stru1.age = 30;
    printf("name = %s , age = %d\n", stru1.name, stru1.age);

    struct stru *pstru = &stru1;
    printf("name = %s\n", pstru->name);
}

// 结构的合法操作只有几种：作为一个整体复制和赋值，通过&运算符取地址，访问其成员。
// 其中，复制和赋值包括向函数传递参数以及函数返回值。结构之间不可进行比较。
struct stru makestru(char *name, int age);
void tostring(struct  stru s);
void changestru(struct stru *s);
struct stru *returnStruPointer(struct stru *s);

void learn_struct_func() {
    struct stru temp = makestru("Lucy", 22);
    printf("p2 = %p\n", &temp);

    tostring(temp);
    printf("1--- %s\n", temp.name);
    changestru(&temp);
    printf("2--- %s\n", temp.name);

    struct stru *temp2 = returnStruPointer(&temp);
    printf("3 -- %s, p = %p\n", temp2->name, temp2);
}

struct stru makestru(char *name, int age) {
    struct stru temp;
    temp.name = name;
    temp.age = age;
    printf("p1 = %p\n", &temp);
    return temp;
}

void tostring(struct  stru s) {
    printf("p3 = %p, name = %s\n", &s, s.name);
    s.name = "jack";
}

void changestru(struct stru *s) {
    printf("p4 = %p\n", s);
    s->name = "jack";
}

// 返回一个指向 struct stru 类型的指针
struct stru *returnStruPointer(struct stru *s) {
    printf("p5 = %p\n", s);
    s->name = "Lucy";
    return s;
}

// 结构数组
// 每一个元素都是一个结构
void learn_struct_array() {
    struct stru arr[] = {
        {"dazuo", 24},
        {"lucy", 24},
    };

    printf("0 = %s\n", arr[0].name);
    printf("1 = %s\n", arr[1].name);

    // 通过指针访问
    printf("2 = %s\n", (*arr).name);
    printf("3 = %s\n", (arr + 1)->name);
}
