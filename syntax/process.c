// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void wait_awhile();
void waitpid_awhile(pid_t);
void zombie_process();
static void sig_child(int signo);

// 进程 ID
//  程序（program）是一个存储在磁盘上某个目录中的可执行文件。程序的执行实例被称为进程（process）。
//  UNIX系统确保每个进程都有一个唯一的数字标识符，称为进程ID（process ID）。
//
// 进程层次
//  产生新进程的那个进程称为父进程;新创建的进程则被称为子进程。每个进程都由另一个进程创建(当然除了init进程)。
//  因此，每个子进程都有一个父进程。这个关系在每个进程的父进程ID(ppid)中记录，它是父进程的 pid。
//
//  每个进程都属于一个用户和组。这个所有权用来控制资源的访问权限。每个子进程都继承父进程的用户和组属性。
//  每个进程同时还属于一个进程组，进程组只是简单地表示进程之间的关系，子进程通常和父进程属于同一个进程组。
//
// 获得进程ID和父进程ID，两个调用都不会返回错误。
void learn_pid() {
    printf("ppid = %d, subprocess pid = %d\n", getppid(), getpid());
}

// 运行新进程
// 在Unix中，创建一个新进程的动作被分离成：装载程序到内存、和执行程序镜像两个动作。一个系统调用(实际上，一个家族调用中的一个)装载
// 二进制程序到内存，替换地址空间中原有的内容，然后开始执行新程序。这就是执行新程序的过程，这个功能由 exec 家族调用提供。
//
// 成功时exec系统调用没有返回;失败时调用返回-1，并设置errno。
//
// 成功调用 execl()不仅仅改变地址空间和进程镜像，还有其它一些进程属性:
//  任何未决的信号都将丢失。
//  任何进程捕获的信号都恢复到默认行为，因为信号处理器在进程地址空间中已经不存在了。
//  任何内存锁都将被丢弃。
//  大部分线程属性恢复为默认值。
//  大部分进程统计数据重置。
//  与进程内存相关的任何东西，包括任何映射文件，都将被丢弃。
//  用户空间存在的任何东西，包括C库(如 atexit()行为)，都将被丢弃。
//
// 但是进程也有许多属性不会改变。例如pid、父进程pid、优先级、以及拥有者用户和组，都保持不变。
void learn_exec() {
    execlp("ls","-l", NULL);
}

// fork对父进程返回新的进程ID，对子进程则返回0。因为 fork 创建一个新进程，所以说它被调用一次（由父进程），
// 但返回两次（分别在父进程中和子进程中）。错误时子进程将不会被创建，fork()返回-1，并设置errno。
//
// 子进程和父进程的处理方式几乎相同，除了以下必要的区别:
//  子进程的pid是新分配的，因此肯定与父进程pid不相同。
//  子进程的ppid被设置为父进程的pid。
//  子进程的资源统计信息重置为0。
//  清除子进程的所有未决信号，子进程不继承父进程的信号。
//  子进程不继承父进程已获得的任何文件锁。
void learn_fork() {
    pid_t pid = fork();
    printf("subprocess pid = %d\n", pid);
    if (pid < 0) {
        perror("error cause");
        return;
    } else if (pid == 0) {
        printf("ppid = %d, subprocess pid = %d\n", getppid(), getpid());
        exit(0);
    }
    printf("main process end\n");
}

// 标准函数来终止当前进程
// void exit (int status);
// status 参数用来指示进程的退出状态。在Linux中，0一般代表成功;非 0 值，例如 1 或-1 对应于失败。
// 在终止进程前，C库执行下列停止步骤，按顺序进行:
//  1.调用所有通过 atexit()或者 on_exit()注册的函数，按注册相反的顺序依次调用。
//  2.flush所有打开的标准 I/O 流。
//  3.移除所有通过 tmpfile()函数创建的临时文件。
//
// 这些步骤完成了进程退出时在用户空间需要做的所有事情，于是 exit()调用系统调用 _exit()来指示内核处理进程终止剩下的工作:
// 当进程退出时，内核清理进程不再使用的那些资源。包括但不限于:已分配内存、打开 文件、系统 V 信号量。清理之后，内核销毁进程并通知父进程子进程已死。
void learn_exit() {
}

void out() {
    printf("atexit() succeeded!\n");
}

// 成功调用 atexit()注册指定的函数，在进程正常终止时调用：例如进程通过 exit()或者从 main()函数返回。
// 如果进程调用了exec函数，则已注册的函数列表将被清空(因为这些函数 在新的进程地址空间中不再存在)。如果
// 进程通过信号终止，则已注册的函数不会被调用。
void learn_atexit() {
    atexit(out);
}

// 等待子进程终止
// 早期的 Unix 设计者决定当子进程在父进程之前死亡时，内核应该把子进程置为一个特殊的进程状态。在这种状态
// 下的进程被称为僵尸进程(zombie)。只有极少量的进程信息被保留——内核基本数据结构中可能有用的那部分数据。
// 在这种状态下的进程等待 父进程询问自己的状态(这个过程被称为等待僵尸进程)。只有父进程获取了已终止子进程
// 保留的信息之后，僵尸进程才会最终消失。
//
// 直到父进程通过wait/waitpid来取时才释放。但这样就导致了问题，如果进程不调用wait/waitpid的话，那么
// 保留的那段信息就不会释放，其进程号就会一直被占用，但是系统所能使用的进程号是有限的，如果大量的产生僵死
// 进程，将因为没有可用的进程号而导致系统不能产生新的进程。此即为僵尸进程的危害，应当避免。

/*
    孤儿进程：一个父进程退出，而它的一个或多个子进程还在运行，那么这些子进程将成为孤儿进程。孤儿进程将
            被init进程（进程号为1）所收养，并由init进程对它们完成状态收集工作。

    僵尸进程：一个进程使用fork创建子进程，如果子进程退出，而父进程并没有调用wait或waitpid获取子进程
            的状态信息，那么子进程的进程描述仍然保存在系统中。这种进程称之为僵尸进程。

    僵尸进程解决办法
    1）通过信号机制：
       子进程退出时向父进程发送SIGCHILD信号，父进程处理SIGCHILD信号。在信号处理函数中调用wait进行处理僵尸进程。
    2）fork两次：
       《Unix 环境高级编程》8.6节说的非常详细。原理是将子进程成为孤儿进程，从而其的父进程变为init进程，通过init进程可以处理僵尸进程。
*/
void learn_wait() {
    signal(SIGCHLD, sig_child);

    pid_t pid = fork();
    if (pid < 0) {
        perror("error cause");
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

// 子进程退出时向父进程发送SIGCHILD信号，父进程处理SIGCHILD信号。在信号处理函数中调用wait进行处理僵尸进程。
static void sig_child(int signo) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated.\n", pid);
    }
}

// 运行并等待新进程
// 成功时函数的返回值是命令的返回状态，和 wait()的返回一样。
void learn_system() {
    int status = system("echo 123");
    printf("status = %d\n", status);
}

void learn_process() {
    learn_system();
}
