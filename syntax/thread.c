// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// FreeBSD使用指向线程数据结构的指针作为它的线程ID。
// Linux线程ID是用无符号长整型来表示。
//
// Linux是用单独的进程实现每个线程的，这使得它很难与POSIX线程的行为匹配。
void printtid(char *s) {
    pthread_t tid = pthread_self();
    // printf("%p", tid);
    printf("%s pid = %d, tid = %lu\n", s, getpid(), (unsigned long)tid);   // 将指针16进制地址，转换为10进制输出
}

void *start_fn(void *arg) {
    printf("arg = %d\n", *(int *)arg);
    printtid("new thread: ");
    return NULL;
}

// 新创建的线程从 start_fn 函数的地址开始运行，该函数只有一个无类型指针参数 arg。
// 如果需要向 start_fn 函数传递一个参数，那么需要把这个参数的地址作为 arg 参数传入。

// pthread函数在调用失败时通常会返回错误码，并不像会设置errno。
// 每个线程都提供error的副本，这只是为了与使用errno的现有函数兼容。
void learn_pthread() {
    pthread_t ntid;

    int arg = 99;
    int err = pthread_create(&ntid, NULL, start_fn, &arg);
    if (err != 0) {
        printf("can't create thread");
        exit(EXIT_FAILURE);
    }
    printtid("main thread: ");

    sleep(1);
}
