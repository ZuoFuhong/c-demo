// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// 1.程序（program）是一个存储在磁盘上某个目录中的可执行文件。内核使用 exec 函数（7个 exec 函数之一）将程序读入内存，并执行程序。
// 2.程序的执行实例被称为进程（process）。UNIX系统确保每个进程都有一个唯一的数字标识符，称为进程ID（process ID）。
// 3.用于进程控制的主要函数：fork、exec和waitpid。（exec 函数有7种变体，但经常把它们统称为 exec 函数。）
void learn_process() {
    // fork对父进程返回新的进程ID，对子进程则返回0。因为 fork 创建一个新进程，所以说它被调用一次（由父进程），
    // 但返回两次（分别在父进程中和子进程中）。
    pid_t pid = fork();
    printf("subprocess pid = %d\n", pid);
    if (pid < 0) {
        printf("fork error\n");
        return;
    } else if (pid == 0) {
        printf("subprocess pid = %d\n", getpid());
        sleep(2);
        return;
    }

    // 等待子进程终止，其参数指定要等待的进程（即pid参数是子进程ID），waitpid函数返回子进程的终止状态（status变量）
    int status;
    pid = waitpid(pid, &status, 0);
    printf("process pid = %d, subprocess pid = %d, stauts = %d", getpid(), pid, status);
}

// 用exec函数可以把当前进程替换为一个新进程，且新进程与原进程有相同的PID。
void learn_exec() {
    execlp("ls","-l", NULL);
}
