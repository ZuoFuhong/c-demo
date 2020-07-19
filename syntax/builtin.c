//
// Created by dazuo on 2020/6/12.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

int FILE_FD = 999;

int getRandomNum(int seed) {
    srand(seed);
    int c = rand(); // NOLINT(cert-msc30-c, cert-msc50-cpp)
    return c;
}

void learn_stdlibh() {
    double a = strtod("123", NULL);
    printf("%.2f\n", a);

    printf("%d\n", getRandomNum(2));
}

void learn_strtol() {
    // 当 base 的值为 0 时，默认采用 10 进制转换，但如果遇到 '0x' / '0X' 前置字符则会使用 16 进制转换，遇到 '0' 前置字符则会使用 8 进制转换。
    long a = strtol("0x123", NULL, 0);
    long b = strtol("01111011", NULL, 2);
    long t = strtol("123", NULL, 10);
    long e = strtol("0123", NULL, 8);
    long h = strtol("0x123", NULL, 16);
    printf("a = %ld, b = %ld, t = %ld, e = %ld, h = %ld", a, b, t, e, h);
}

// 实现：内置stdlib.h#strtol()
int atoi(const char s[]) {
    int i, n;
    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9';i++){
        n = n * 10 + (s[i] - '0');
    }
    return n;
}

void learn_stream_input() {
    int c;
    // read a character
    while ((c = getchar()) != EOF) {
        // output a character
        putchar(c);
    }
}

void learn_ctypeh() {
    char letter = 'a';
    printf("%c", (char)toupper(letter));

    // 若字符 letter 是数字，则返回一个非0，否则返回0
    printf("%d", isdigit(letter));
}

// 字符串操作函数
void learn_stringh() {
    char s[11] = "hello";
    printf("l = %ld\n", strlen(s));
    printf("%d\n", s[5] == '\0');

    char s2[] = "world";
    // 将s2指向的字符串连接到s指向的字符串的末尾
    strcat(s, s2);
    printf("%d\n", s[5] == '\0');
    printf("%d\n", s[10] == '\0');
}

// 数学函数
void learn_mathh() {
    int a = -1;
    printf("%d", abs(a));

    double b = pow(2, 3);
    printf("%g", b);
}

// 时间日期格式化
void learn_time() {
    // 返回自纪元 Epoch（1970-01-01 00:00:00 UTC）起经过的时间，以秒为单位。如果 seconds 不为空，则返回值也存储在变量 seconds 中。
    time_t ticks = time(NULL);
    printf("%ld\n", ticks);

    // 把日期和时间转换为字符串
    printf("%s", ctime(&ticks));

    // 时间格式化
    struct tm *p = localtime(&ticks);
    char now[100];
    strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S", p);
    printf("now = %s\n", now);

    // 使用 gettimeofday 获取更精准的时间
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("second：%ld \n", tv.tv_sec);
    printf("millisecond: %ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void alarm_handler(int signo) {
    printf("alarm clock: %d\n", signo);
}

// 调用这个函数在 seconds 秒实时时间过去后，向调用进程发送 SIGALRM 信号。如果之前预定的信号未决，
// 调用取消 alarm，并用新请求的 alarm 替换它，然后返回前一个 alarm 剩余的秒数。如果seconds为0，
// 之前的 alarm(如果有)被取消，并且不会有新的 alarm 预定。
void learn_alarm() {
    signal(SIGALRM, alarm_handler);
    alarm(2);

    // 让进程暂停直到信号出现
    pause();
}

// int setitimer (int which, const struct itimerval *value, struct itimerval *ovalue);
// 其中：
//   which：定时器的类型
//   it_value：指定期满时间。一旦it_value指定的时间过去，内核将以it_interval提供的时间重新装备定时器。
//   it_value：到达0，它将被设置为it_interval。如果定时器到期并且 it_interval 为 0，定时器不会重装。
//             类似的，如果定时器的 it_value 设置为 0，定时器将停止而不重装。
// 函数在成功时都返回 0;出错返回-1，并设置 errno。
//
// 如下设置一个SIGALRM 信号处理器，然后以 5 秒初始过期时间，以及 1 秒的后继间隔装备定时器。
void learn_timer() {
    signal(SIGALRM, alarm_handler);

    struct itimerval delay;
    delay.it_value.tv_sec = 5;
    delay.it_value.tv_usec = 0;
    delay.it_interval.tv_sec = 1;
    delay.it_interval.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &delay, NULL);
    if (ret) {
        perror("setitimer");
        exit(EXIT_FAILURE);
    }

    unsigned int v = 10;
    while (v > 0) {
        v = sleep(v);
    }
}
