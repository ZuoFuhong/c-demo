//
// Created by dazuo on 2020/6/12.
//

/* 逆波兰计算器 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100   // 操作数或运算符的最大长度
#define NUMBER '0'  // 标识找到一个数

int getop(char s[]);
void push(double);
double pop(void);

void calculator_main() {
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(strtof(s, NULL));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0) {
                    push(pop() / op2);
                } else {
                    printf("error: zero divisor\n");
                }
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unknow command %s\n", s);
                break;
        }
    }
}

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

#define MAXVAL 100  // 栈的最大深度

int sp = 0;         // 下一个空闲栈位置
double val[MAXVAL]; // 值栈

void push(double f) {
    if (sp < MAXVAL) {
        val[sp++] = f;
    } else {
        printf("error: stack full, can't push %g\n", f);
    }
}

double pop() {
    if (sp > 0) {
        return val[--sp];
    } else {
        printf("error: stack empty\n");
        return 0.0;
    }
}
