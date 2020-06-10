#include <stdio.h>
#include <string.h>
#include "main.h"

#define CODE "ABC"
#define STEP 20

int main() {
    // TODO: Page 33
    learn_stream_input();
}

void learn_data_type() {
    char ch = 'a';
    int num = 10;
    double money = 2.3;
    printf("%.2f", money / 3);
    printf("\n");
    printf("%x", 0x12);
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
    enum Week {
        Mon = 1, Thes = 2, Wed = 3
    } day = Mon;

    printf("%d, sizeof = %ld", day, sizeof(day));
}

void learn_array(int *list, int len) {
    for (int i = 0; i < len; ++i) {
        if (i == 1) {
            *(list + i) = 2;
        }
    }
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
