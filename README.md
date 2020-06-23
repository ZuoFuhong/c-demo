## c Learning notes

回归计算机的起点，0和1的世界

## 常用命令

```
$ cc hello.c -o hello           // 无选项，经过预处理、汇编、编译并连接形成可执行文件

$ cc -E hello.c -o hello.i      // 预处理
$ cc -S hello.i -o hello.s      // 编译为汇编语言
$ cc -c hello.s -o hello.o      // 汇编
$ cc hello.o -o hello           // 连接，生成可执行文件
```
