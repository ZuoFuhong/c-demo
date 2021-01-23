## c Learning notes

回归计算机的起点，0和1的世界

## 常用命令

```
$ cc hello.c -o hello           // 无选项，经过预处理、编译、汇编并链接形成可执行文件

$ cc -E hello.c -o hello.i      // 预处理阶段。预处理器（cpp）根据以字符#开头的命令，修改原始的C程序。
$ cc -S hello.i -o hello.s      // 编译阶段。编译器（ccl）将文本文件 hello.i 翻译成文本文件 hello.s，它包含一个汇编语言程序。
$ cc -c hello.s -o hello.o      // 汇编阶段。汇编器（as）将hello.s翻译成机器语言指令，把这些指令打包成一种叫做可重定位目标程序的格式，并将结果保存在目标文件hello.o中。
$ cc hello.o -o hello           // 链接阶段。链接器（ld）负责处理多个目标文件以及所需的库文件，生成一个可执行文件。
```
