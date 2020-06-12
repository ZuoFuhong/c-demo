//
// Created by dazuo on 2020/6/12.
//
#include <stdio.h>
#include <ctype.h>
#include "calc.h"

// 获取下一个运算符或操作数
int getop(char s[]) {
    int i = 0, c;
    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t') {
            break;
        }
        if (!isdigit(c) && c != '.') {
            return c;
        }
        s[i++] = (char)c;
    }
    s[i] = '\0';    // 标识字符串的结束
    return NUMBER;
}
