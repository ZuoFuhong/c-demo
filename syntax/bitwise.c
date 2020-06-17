//
// Created by dazuo on 2020/6/17.
//
#include <stdio.h>

// C语言位级运算
// Java实现笔记：https://github.com/ZuoFuhong/bulb/blob/master/bulb-jdk-basic/src/main/java/com/maxzuo/basic/BitwiseLogicalOperatorExample.java

void bit_and_operator();

void learn_bitwise() {
    bit_and_operator();
}

// 位与运算符
void bit_and_operator() {
    /*
          位与运算符为 &，其运算规则是：参与运算的数字，低位对齐，高位不足的补零，如果对应的二进制位同时为 1，那么计算结果才为 1，
          否则为 0。因此，任何数与 0 进行按位与运算，其结果都为 0。
            0000 0000 0000 0000 0000 0000 0000 1011    -> 11
          &
            0000 0000 0000 0000 0000 0000 0000 1001    -> 9
            ___________
            0000 0000 0000 0000 0000 0000 0000 1001    -> 9
     */
    unsigned int l = 11u;
    unsigned int r = 9u;
    printf("%d\n", l & r);
}
