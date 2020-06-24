// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <sys/wait.h>

// 信号（signal）用于通知进程发生了某种情况。例如，若某一进程执行了除法操作，其除数为0，
// 则将名为SIGFPE（浮点异常）的信号发送给该进程。进程有以下3种处理信号的方式。
// 1）忽略信号。有些信号表示硬件异常，例如，除以0或访问进程地址以外的存储单元等，因为这些异常产生的后果不确定，所以不推荐使用这种处理方式。
// 2）按系统默认的凡是处理。对于除数为0，系统默认方式是终止该进程。
// 3）提供一个函数，信号发生时调用该函数，这被称为捕捉该信号。
void sig_int(int signo);

void learn_signal() {
    // 注册信号监听
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        printf("signal error\n");
        return;
    }

    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
}

void sig_int(int signo) {
    printf("interrupt signal: %d\n", signo);
}
