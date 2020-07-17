// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.
#include <stdio.h>

/*
    标准C库的标准I/O，是一个用户缓冲I/O，作为标准C库的一部分提供。
*/

// 标准I/O函数并不直接操作文件描述符，而是使用自己定义的唯一标识，就是文件指针。在C库里，文件指针映射至一个文件描述符。
// 文件指针是指向FILE类型的指针，在<stdio.h> 中定义。
// 按照标准I/O的说法，一个打开的文件被称为流。流可以打开用来读取(输入流)、写入(输 出流)、或者二者一起(输入/输出流)。
//
// 成功时fopen()返回一个合法的FILE指针；失败时返回NULL，并设置errno为相应的错误代码。
void use_fopen() {
    FILE *fp = fopen("info.log", "w+");
    if (fp == NULL) {
        perror("error cause");
        return;
    }

    // 成功时 fileno()返回 stream 相关联的文件描述符; 失败时返回-1。只有在给定的流无效时才可能发生失败，
    // 这时候函数将设置 errno 为 EBADF。
    int fd = fileno(fp);
    printf("%d", fd);

    // 标准I/O库提供一个接口来把用户缓冲写出到内核，确保通过 write()写出的所有数据被刷新到流中。调用 fflush()后，
    // 流 stream 中的任何未写入数据都被 flushed 到内核。如果 stream 为 NULL，则当前进程所有已打开的输入流都将
    // 被 flushed。成功时 fflush()返回 0;失败时返回 EOF，并设置errno 为适当的值。
    fflush(fp);

    // 关闭指定的流，所有已缓冲但尚未写入的数据会首先被flush。成功时fclose()返回 0；
    // 失败时返回 EOF 并设置errno为相应的错误代码。
    fclose(fp);
}

// 标准C库实现了多个函数从打开的流中读取数据
// 1.fgetc()函数从 stream 中读取下一个字符，然后把它从 unsigned char 转换为 int 并返回。执行
//   转换是为了拥有足够的范围来通知到达文件末尾或者出现错误: 这时候返回 EOF。
// 2.fgets()函数从指定的流中读取一个字符串，函数最多从stream中读取 n减1个字节，并把结果存储在str中。
//   空字符（\0）会存储在读取的字节后面。遇到 EOF 或者换行字符时，读取操作停止。如果读取到了换行字符，
//   则 \n 也会存储在str中。
void use_fget() {
    FILE *fp = fopen("info.log", "r");
    printf("%c\n", fgetc(fp));

    char buf[5];
    printf("%s\n", fgets(buf, sizeof(buf), fp));
    printf("%d", buf[4] == '\0');   // true
}

// 读取二进制数据
// 语法：size_t fread (void *buf, size_t size, size_t nr, FILE *stream);
// 调用 fread()最多将从 stream 读取 nr 个元素，每个 size 字节，到buf指向的缓冲区中。
// 文件指针将更新所读取的字节数。
//
// 函数返回读取的元素个数（而不是读取的字节数）。fread()返回小于 nr 的值时表示发生错误或者到达EOF。
void use_fread() {
    FILE *fp = fopen("info.log", "r");
    char buf[11];
    size_t size = fread(buf, sizeof(buf) - 1, 1, fp);
    printf("%s, %ld\n", buf, size);
    printf("%d", buf[10] == '\0');   // true
}

// fputc()函数写入 c 指定的字节(转换为 unsigned char)到 stream 流。成功结束时，函数 返回 c;
// 否则返回 EOF，并且设置 errno 为适当的值。
//
// 调用 fputs()写入 str 指向的 null 结尾的字符串到 stream 流中。成功时，fputs()返回一个非负整数;失败时返回 EOF。
void use_fput() {
    FILE *fp = fopen("info.log", "w");
    fputc('e', fp);

    fputs("hello", fp);
}

// 写入二进制数据
// 调用 fwrite()将从 buf 指向的数据中，最多写入 nr 个元素到 stream，每个大小 size 字节。文件指针会增加写入的总字节数。
// 函数成功时返回成功写入的元素个数(而不是字节数)，小于 nr 的返回值表示发生错误。
//
// 注意：由于变量大小、对齐等许多原因，一个程序写入的二进制数据，可能无法被另一个程序读取。这些东西只有在特定的机器类型和特定的ABI下才能确保不变。
void use_fwrite() {
    FILE *fp = fopen("info.log", "w");
    char buf[] = "helloworld";
    fwrite(buf, sizeof(buf) - 1, 1, fp);    // 丢弃尾部的（\0）字符
}

// 标准 I/O 提供了一组接口，在功能上基本等同于系统调用 lseek()。fseek()函数是标准I/O最常用的seek接口，
// 根据 offset 和 whence 来操作 stream 流的文件位置。
//
// int fseek(FILE *stream, long offset, int whence);
// 成功结束时，fseek()返回0，清除EOF指示器，并且撤消 ungetc()的效果(如果有); 错误时返回-1，并设置errno为适当的值。
//
// void rewind (FILE *stream);
// 调用 rewind(stream)重置位置到流的开始位置。它等同于: fseek(stream, 0, SEEK_SET); 除了后者同时会清除错误指示器之外。
// 注意 rewind()没有返回值，因此不能直接与错误情况交互。调用者如果希望确认错误是 否存在，应该在调用函数之前先清除 errno，
// 然后在函数结束之后检查 errno 变量的值是否非 0。
//
// long ftell (FILE *stream);
// ftell()函数返回流 stream 的当前位置，出错时返回-1，并且设置 errno。
//
// int fgetpos (FILE *stream, fpos_t *pos);
// 成功时 fgetpos()返回0，并且把流 stream 的当前位置值存入 pos。失败时返回-1，并设置 errno。
void use_fseek() {
    FILE *fp = fopen("info.log", "w+");
    if (fp == NULL) {
        perror("error cause");
        return;
    }

    fputs("hello world", fp);
    long pos = ftell(fp);
    printf("pos = %ld\n", pos);

    // 将位置指针移动到文件开头
    rewind(fp);
    printf("%c\n", fgetc(fp));

    fpos_t pos2;
    fgetpos(fp, &pos2);
    printf("pos2 = %lld", pos2);
}

// 标准I/O提供了两个接口。函数 ferror()检测 stream 的错误指示器是否被设置:
// int ferror (FILE *stream);       错误指示器被其它标准I/O接口在发生错误时设置。如果错误指示器被设置，函数返回一个非 0 值，否则返回 0。
// int feof (FILE *stream);         EOF指示器被其它标准I/O接口在到达文件末尾时设置。如果EOF指示器被设置，函数返回一个非 0 值，否则返回 0。
// void clearerr (FILE *stream);    clearerr()函数清除stream流的错误和EOF指示器。
void use_ferror() {
}
