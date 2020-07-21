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

// ---------------------------------------- thread_create ---------------------------------------- //

void *start_fn(void *arg) {
    printf("arg = %d\n", *(int *)arg);
    printtid("new thread: ");
    return NULL;
}

// 新创建的线程从 start_fn 函数的地址开始运行，该函数只有一个无类型指针参数 arg。
// 如果需要向 start_fn 函数传递一个参数，那么需要把这个参数的地址作为 arg 参数传入。

// pthread函数在调用失败时通常会返回错误码，并不像会设置errno。
// 每个线程都提供error的副本，这只是为了与使用errno的现有函数兼容。
void use_pthread_create() {
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

// ---------------------------------------- thread_join ---------------------------------------- //

void *start_fu2() {
    printtid("new thread:");

    // 单个线程可以通过3种方式退出：
    // 1）线程可以简单地从启动例程中返回，返回值是线程的退出码。
    // 2）线程可以被同一进程中的其它线程取消。
    // 3）线程调用pthread_exit。
    pthread_exit((void *)2);
}

// 调用pthread_join函数可以等待指定的线程终止，可以获得线程的终止状态。
void use_pthread_join() {
    pthread_t ntid;
    int err = pthread_create(&ntid, NULL, start_fu2, NULL);
    if (err != 0) {
        printf("can't create thread");
        exit(EXIT_FAILURE);
    }

    void *tret;
    err = pthread_join(ntid, &tret);
    if (err != 0) {
        printf("can't join with thread");
        exit(EXIT_FAILURE);
    }
    printf("thread exit code %ld\n", (long)tret);
}

// ---------------------------------------- thread_clean_push ---------------------------------------- //

void cleanup_handler(void *arg) {
    printf("cleanup: %s\n", (char *)arg);
}

void *cleanup_fn() {
    printf("thread start\n");
    // 类似进程的 atexit 注册线程清理处理程序
    pthread_cleanup_push(cleanup_handler, "thread 1 first handler")};
    printf("thread complete\n");

    // 注销
    // pthread_cleanup_pop(0);

    // 如果线程是通过启动例程返回而终止的话，它的清理程序就不会被调用。
    pthread_exit(0);
}

void use_thread_cleanup() {
    pthread_t ntid;
    int err = pthread_create(&ntid, NULL, cleanup_fn, NULL);
    if (err != 0) {
        printf("can't create thread");
        exit(EXIT_FAILURE);
    }
    pthread_join(ntid, NULL);
}

// ---------------------------------------- thread_mutex_lock ---------------------------------------- //

struct foo {
    int             count;
    pthread_mutex_t lock;
    int             id;
};

// 互斥量变量使用 pthread_mutex_t 数据类型来表示，在使用互斥变量以前，必须首先对它进行初始化，
// 可以把它设置为常量 PTHREAD_MUTEX_INITIALIZER（只适合用于静态分配的互斥量），也可以通过
// 调用 pthread_mutex_init 函数进行初始化。如果动态分配互斥量（例如，通过调用malloc函数），
// 在释放内存前需要调用 pthread_mutex_destroy。
//
// 要用默认的属性初始化互斥量，只需要把 attr 设为NULL。
struct foo *foo_alloc(int id) {
    struct foo *fp = malloc(sizeof(struct foo));
    if (fp != NULL) {
        fp -> count = 1;
        fp -> id = id;
        if (pthread_mutex_init(&fp -> lock, NULL) != 0) {
            free(fp);
            return NULL;
        }
    }
    return fp;
}

void foo_hold(struct foo *fp) {
    pthread_mutex_lock(&fp -> lock);
    fp -> count++;
    pthread_mutex_unlock(&fp -> lock);
}

void foo_release(struct foo *fp) {
    pthread_mutex_lock(&fp -> lock);
    if (--fp -> count == 0) {
        pthread_mutex_unlock(&fp -> lock);
        pthread_mutex_destroy(&fp -> lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&fp -> lock);
    }
}

// 使用 pthread 的互斥接口可以确保同一时间只有一个线程访问数据。互斥量（mutex）本质上说是一把锁，
// 在访问共享资源前对互斥量进行设置（加锁），在访问完成后释放（解锁）互斥量。对互斥量进行加锁以后，
// 任何其他试图再次对互斥量加锁的线程都会被阻塞直到当前线程释放该互斥锁。如果释放互斥量时有一个以上
// 的线程阻塞，那么所有该锁上的阻塞线程都会变成可运行状态，第一个变成运行的线程就可以对互斥量加锁，
// 其它线程就会看到互斥量依然是锁着的，只能回去再次等待它重新变成可用。
//
// 补充：
//  1.带有超时的互斥锁 pthread_mutex_timedlock 允许绑定线程超时时间（Mac不支持，Linux下支持）
//  2.读写锁可以有3种状态；读模式下加锁状态，写模式下加锁状态，不加锁状态。一次只有一个线程可以占有写模式的读写锁，
//    但是多个线程可以同时占有读模式的读写锁。
//  3.带有超时的读写锁 pthread_rwlock_timedwrlock 允许绑定线程超时时间（Mac不支持，Linux下支持）
void use_thread_mutex_lock() {
    struct foo *fp = foo_alloc(10);
    printf("id = %d, count = %d\n", fp -> id, fp -> count);
    foo_hold(fp);
    printf("id = %d, count = %d\n", fp -> id, fp -> count);
    foo_release(fp);
    printf("id = %d, count = %d\n", fp -> id, fp -> count);
}

// ---------------------------------------- thread_cond_wait ---------------------------------------- //

// 条件变量是线程可用的另一种同步机制。条件变量给多个线程提供了一个会合的场所。条件变量与互斥量一起使用时，允许线程
// 以无竞争的方式等待特定的条件发生。
//
// 条件本身是由互斥量保护的。线程在改变条件状态之前必须首先锁住互斥量。其它线程在获得互斥量之前不会察觉到这种改变，
// 因为互斥量必须在锁定以后才能计算条件。
//
// 在使用条件变量之前，必须先对它进行初始化。由pthread_cond_t 数据类型表示的条件变量，可以使用两种方式进行初始化，
// 可以把常量PTHREAD_COND_INITIALIZER赋给静态分配的条件变量。如果条件变量是动态分配的，则需要pthread_cond_init
// 函数对它进行初始化。
//
// 在释放条件变量底层的内存空间之前，可以使用pthread_cond_destroy 函数对条件变量进行反初始化。
struct msg {
    struct msg *next;
    int    id;
};
struct msg *last;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void enqueue_msg(struct msg *m) {
    pthread_mutex_lock(&qlock);
    m -> next = last;
    last = m;
    pthread_mutex_unlock(&qlock);
    // 至少唤醒一个等待该条件的线程
    pthread_cond_signal(&qready);
}

// 传递给pthread_cond_wait 的互斥量对条件进行保护。调用者把锁住的互斥量传给函数，函数然后自动把调用线程放到
// 等待条件的线程列表上，对互斥量解锁。这就关闭了条件检查和线程进入休眠等待条件改变这两个操作之间的时间通道，
// 这样线程就不会错误条件的任何变化。pthread_cond_wait 返回时，互斥量再次被锁住。
void *process_msg() {
    struct msg *m;
    for (;;) {
        pthread_mutex_lock(&qlock);
        while (last == NULL) {
            pthread_cond_wait(&qready, &qlock);
        }
        m = last;
        last = m -> next;
        pthread_mutex_unlock(&qlock);

        // now process the message mp
        printf("the message id = %d\n", m -> id);
        fflush(stdout);
    }
}

void use_pthread_cond() {
    pthread_t ntid;
    int err = pthread_create(&ntid, NULL, process_msg, NULL);
    if (err != 0) {
        printf("can't create thread");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; i++) {
        struct msg m = {NULL, i};
        enqueue_msg(&m);
        sleep(1);
    }
    pthread_join(ntid, NULL);
}

// ---------------------------------------- pthread barrier ---------------------------------------- //

// 屏障是用户协调多个线程并行工作的同步机制。屏障允许每个线程等待，直到所有的合作线程都到达某一点，
// 然后从该点继续执行。pthread_join函数就是一种屏障，允许一个线程等待，直到另一个线程退出。
//
// 使用 pthread_barrier_init 函数对屏障进行初始化，用 thread_barrier_destory函数反初始化。
// 调用 pthread_barrier_wait 的线程在屏障计数未满足条件时，会进入休眠状态，如果该线程是最后一个
// 调用 pthread_barrier_wait 的线程，就满足了屏障计数，所有的线程都被唤醒。
void use_pthread_barrier() {
    // pthread_barrier_init();
    // pthread_barrier_wait()
}

void learn_pthread() {
    use_pthread_barrier();
}
