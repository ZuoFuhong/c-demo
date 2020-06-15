//
// Created by dazuo on 2020/6/12.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int FILE_FD = 999;

void learn_stdlib() {
    double a = strtod("123", NULL);
    long b = strtol("123", NULL, 10);
    printf("%.2f\n", a);
    printf("%ld\n", b);
}

void learn_stream_input() {
    int c;
    // read a character
    while ((c = getchar()) != EOF) {
        // output a character
        putchar(c);
    }
}

void learn_tolower() {
    char letter = 'a';
    printf("%c", (char)toupper(letter));
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