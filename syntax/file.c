//
// Created by dazuo on 2020/6/15.
//
#include <stdio.h>
#include <sys/stat.h>

void learn_file() {
    char file_name[] = "info.log";
    FILE *fp = fopen(file_name, "w+");

    fputs("hello world", fp);

    // 将位置指针移动到文件开头
    rewind(fp);
    int c = fgetc(fp);
    printf("%c", c);

    // 断开由fopen函数建立的文件指针和外部名之间的连接，并释放文件指针以供其它文件使用。
    // 因为大多数操作系统都限制了一个程序可以同时打开的文件数，所以，当文件指针不再需要时
    // 就应该释放。对输出文件执行fclose还有另外一个原因：它将缓冲区中由puts函数正在收集
    // 的输出写到文件中。当程序正常终止时，程序会自动为每个打开的文件调用fclose函数。
    fclose(fp);
}

// 文件属性
void learn_stat() {
    char *ptr = NULL;
    struct stat buf;
    lstat("/Users/dazuo/workplace/c-learning-notes", &buf);
    if (S_ISDIR(buf.st_mode)) {
        ptr = "directory";
    }
    printf("%s\n", ptr);
    lstat("/Users/dazuo/workplace/c-learning-notes/README.md", &buf);
    if (S_ISREG(buf.st_mode)) {
        ptr = "regular";
    }
    printf("%s\n", ptr);
}
