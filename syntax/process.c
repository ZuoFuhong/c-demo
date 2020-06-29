// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

// 1.程序（program）是一个存储在磁盘上某个目录中的可执行文件。内核使用 exec 函数（7个 exec 函数之一）将程序读入内存，并执行程序。
// 2.程序的执行实例被称为进程（process）。UNIX系统确保每个进程都有一个唯一的数字标识符，称为进程ID（process ID）。
// 3.用于进程控制的主要函数：fork、exec和waitpid。（exec 函数有7种变体，但经常把它们统称为 exec 函数。）

void wait_awhile();
void waitpid_awhile(pid_t);
void zombie_process();
static void sig_child(int signo);

void learn_process() {
    // 子进程退出时向父进程发送SIGCHILD信号，父进程处理SIGCHILD信号。在信号处理函数中调用wait进行处理僵尸进程。
    signal(SIGCHLD, sig_child);

    // fork对父进程返回新的进程ID，对子进程则返回0。因为 fork 创建一个新进程，所以说它被调用一次（由父进程），
    // 但返回两次（分别在父进程中和子进程中）。
    pid_t pid = fork();
    printf("subprocess pid = %d\n", pid);
    if (pid < 0) {
        printf("fork error\n");
        return;
    } else if (pid == 0) {
        printf("ppid = %d, subprocess pid = %d\n", getppid(), getpid());
        sleep(2);
        exit(0);
    }
    // wait_awhile();
    // waitpid_awhile(pid);

    printf("I am father process.I will sleep four seconds\n");

    // sleep函数使调用进程被挂起，当满足以下条件之一时被唤醒：
    //  1）已经过了seconds所指定的墙上时钟时间。
    //  2）调用进程捕捉到一个信号并从信号处理程序返回。
    //
    // sleep的返回值：
    //  1）在上述第一种情形中，返回值是0
    //  2）当由于捕捉到某个信号sleep提前返回时，返回值是未睡够的时间（所要求的时间减去实际休眠时间）。
    sleep(5);
    printf("father process is exiting.\n");
    return;
}

// 函数wait和waitpid 均返回：若成功则为进程ID，若出错则为0或-1
void wait_awhile() {
    // 如果调用wait的进程没有已终止的子进程，不过有一个或多个子进程仍在运行，那么wait将阻塞
    // 到现有子进程第一个终止为止。
    int statloc;
    pid_t  pid = wait(&statloc);    // 等效于：waitpid(-1, &status, 0);
    printf("process pid = %d, subprocess pid = %d, stauts = %d", getpid(), pid, statloc);
}

void waitpid_awhile(pid_t pid) {
    // 等待子进程停止，当 pid = -1时，指代任意子进程。当 pid > 0时，指定要等待的进程ID，通过statloc指针返回子进程终止状态。

    // 其次，options参数允许指定附加选项。最常用的选项是WNOHANG，它告诉内核在没有已终止子进程时不要阻塞。
    int statloc;
    pid = waitpid(pid, &statloc, 0);
    printf("process pid = %d, subprocess pid = %d, stauts = %d\n", getpid(), pid, statloc);
}

/*
    孤儿进程：一个父进程退出，而它的一个或多个子进程还在运行，那么这些子进程将成为孤儿进程。孤儿进程将
            被init进程（进程号为1）所收养，并由init进程对它们完成状态收集工作。

    僵尸进程：一个进程使用fork创建子进程，如果子进程退出，而父进程并没有调用wait或waitpid获取子进程
            的状态信息，那么子进程的进程描述仍然保存在系统中。这种进程称之为僵尸进程。

    unix提供了一种机制可以保证只要父进程想知道子进程结束时的状态信息，就可以得到。这种机制就是：在每个
    进程退出的时候，内核释放该进程所有的资源，包括打开的文件，占用的内存等。但是仍然为其保留一定的信息
    （包括进程号、退出状态、运行时间等)。直到父进程通过wait/waitpid来取时才释放。但这样就导致了问题，
    如果进程不调用wait/waitpid的话，那么保留的那段信息就不会释放，其进程号就会一直被占用，但是系统所
    能使用的进程号是有限的，如果大量的产生僵死进程，将因为没有可用的进程号而导致系统不能产生新的进程。
    此即为僵尸进程的危害，应当避免。

    僵尸进程解决办法
    1）通过信号机制：
       子进程退出时向父进程发送SIGCHILD信号，父进程处理SIGCHILD信号。在信号处理函数中调用wait进行处理僵尸进程。
    2）fork两次：
       《Unix 环境高级编程》8.6节说的非常详细。原理是将子进程成为孤儿进程，从而其的父进程变为init进程，通过init进程可以处理僵尸进程。
*/
static void sig_child(int signo) {
    pid_t pid;
    int stat;
    // 处理僵尸进程
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated.\n", pid);
    }
    return;
}

// 用exec函数可以把当前进程替换为一个新进程，且新进程与原进程有相同的PID。
void learn_exec() {
    execlp("ls","-l", NULL);
}
