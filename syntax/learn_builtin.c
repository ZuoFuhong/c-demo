//
// Created by dazuo on 2020/6/12.
//
#include <stdio.h>
#include <stdlib.h>

void learn_stdlib() {
    double a = strtod("123", NULL);
    long b = strtol("123", NULL, 10);
    printf("%.2f\n", a);
    printf("%ld", b);
}
