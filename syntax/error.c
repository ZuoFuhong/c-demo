// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <errno.h>
#include <string.h>

// 出错处理
// 当UNIX系统函数出错时，通常会返回一个负值，而且整型变量 errno 通常被设置为具体特定信息的值。
// 例如，open 函数如果成功执行则返回一个非负文件描述符，如出错则返回-1。在open出错时，有大约
// 15种不同的error值（文件不存在、权限问题等）。而有些函数对于出错则使用另一种约定而不是返回负值。
// 例如，大多数返回指向对象指针的函数，在出错时会返回一个 NULL 指针。
// 文件<errno.h>中定义了errno以及可以赋与它的各种常量。这些常量都是以字符 E 开头。

// 出错恢复
// 可将在<errno.h>中定义的各种出错分为两类：致命性的和非致命性的。对于致命性的错误，无法执行恢复
// 动作。最多能做的是在用户屏幕上打印出一条出错消息或者将一条出错消息写入日志文件中，然后退出。对于
// 非致命性的出错，有时可以较妥善地进行处理。
void learn_error() {
    // strerror函数将 errno 映射为一个出错消息字符串，并且返回此字符串的指针
    char *s = strerror(EACCES);
    printf("%s\n", s);

    // perror 函数基于 errno 的当前值，在标准错误上产生一条出错消息，然后返回。
    // 它首先输出由 msg 指向的字符串，然后是一个冒号，一个空格，接着是对应于 errno 值的出错消息，最后是一个换行符。
    errno = ENOENT;
    perror("cause error");
}
