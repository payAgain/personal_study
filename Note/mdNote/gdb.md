# GDB基础操作

[文章来源](https://juejin.cn/post/6844903950697644040)

## 00 介绍

GDB（GNU Debugger）是 UNIX 及 UNIX-like 下的强大调试工具，可以调试 ada, c, c++, asm, minimal, d, fortran, objective-c, go, java,pascal 等语言，这一份指南我们主要以 c 来作为例子。

## 01 基础

### 1.1 判断程序是否可调试

```bash
# gdb helloworld
Reading symbols from helloWorld...(no debugging symbols found)...done.

# gdb helloworld
Reading symbols from helloWorld...done.
复制代码
```

上面一种有 `no debugging symbols found` 表示不可调试，下面是可调式的。

或者 **readelf 查看段信息：**

```ini
1# readelf -S helloworld|grep debug
2  [28] .debug_aranges    PROGBITS         0000000000000000  0000106d
3  [29] .debug_info       PROGBITS         0000000000000000  0000109d
4  [30] .debug_abbrev     PROGBITS         0000000000000000  0000115b
5  [31] .debug_line       PROGBITS         0000000000000000  000011b9
6  [32] .debug_str        PROGBITS         0000000000000000  000011fc
复制代码
```

如果没有输出任何 debug 信息，也不能调试。

### 1.2 开启 gdb 编译

加上 `-g` 选项：

```
gcc -g -o xxx xxx.c
复制代码
```

### 1.3 gdb xxx 进入调试

- `b 行号或函数` 添加断点
- `r` 跑到下一个断点
- `s` 单步跟踪
- `n` 单步执行
- `p` 查看当前程序的运行数据 比如：`p a` 输出a变量的值 输出格式可以设置： 比如`p/d a` 十进制输出a变量的值
- `p array@idx` 可以查看数组 array 中 idx 处的值
- 设置display，比如 `display a` 这样以后每次调试都会输出a变量的值
- `x` 查看内存地址中的值 语法：x/
- `l` 查看原程序代码，`l 9` 列出第9行附件的源码（`l 2,8` 列出2-8行之间的数据），`l func` 列出指定函数附件的源码
- `p x=8` 在调试过程中修改变量x的值，下面生效
- `jump` 实现跳转，可以是文件的行号，也可以是file:line，也可以是+num这种格式 jump address是代码行的内存地址
- `signal` 产生信号量
- `return` 强制返回
- `call` 强制调用
- `until（简写u)` 当在一个循环体内时，运行退出循环
- `until +行号` 运行至某行停住，不仅仅跳出循环
- `finish` 当前函数执行完，并打印函数返回时的堆栈地址和返回值及参数值等信息
- `skip` 在 step 时跳过一些不想关注的函数或者某个文件的代码，如 `skip function add` 表示跳过函数 add，`skip file step.c` 跳过文件 step.c，`info skip` 查看跳过的信息。
- `c` 继续执行 跳到下一个断点
- `bt` 查看堆栈
- `where` 报错时查看哪里出错，与 `bt` 类似
- `info b` 查看断点情况
- `q` 退出
- `ptype` 输出结构体类型
- `info registers` 显示寄存器值， `info all-registers` 显示所有寄存器
- `info breakpoints` 可以查看所有已设置的端点

### 1.5 命令进阶

#### 1.5.1 设断点

1. `info breakpoints` 查看所有断点
2. `b 9` 或者 `b test.c:9` 根据行号设置断点
3. `b func` 根据函数名设置断点
4. `b test.c:9 if b==0` 根据程序某个条件会出现问题，设置该条件断点(这样当出现问题时，会卡主，用来判断是否是该问题)
5. `rbreak print*` 对所有 print 开头的函数都设断点，`rbreak test.c:.` 对test.c 中所有函数设断点
6. `tbreak test.c:9` 设 **临时断点** ，即这个断点只生效一次
7. `ignore 1 30` 忽略某个断点的前面 30 次执行，从第 31 次开始生效，节约时间
8. `watch a` 观察某个值或表达式，什么时候发生变化
9. `disable/enable num` 禁用/启用所有/某个断点
10. `clear` 清除所有断点，用于清除某个函数，某行的断点，如 `clear func` 、`clear linenum`
11. `delete` 删除所有断点，包括watchpoints, catchpoints，用于删除断点号的断点，如 `delete bnum`

#### 1.5.2 查看变量

1. `p 'test.c'::a` 打印某个文件的变量，`p 'main'::b` 打印某个函数定义的变量
2. `p *p@10` 打印指针指向的内容，@后面为打印的长度
3. `p *$.next` 打印链表linkNode的下一个节点内容
4. `p/x c` 按十六进制打印内容(x：十六进制，d：十进制，o：八进制，t：二进制，c：字符格式，f：浮点格式)
5. `x addr` 查看内存地址值
6. `display e` 程序断住显示某个变量的值

#### 1.5.3 编辑源码

启动调试后，不想退出程序而编辑源码，如何做呢？

gdb 模式下用的默认编辑器是 `/bin/ex` ，如果没有或者想换成其他编辑器，如VIM，可以这样：

```javascript
export EDITOR=/usr/bin/vim
复制代码
```

gdb 模式下编辑源码：

```bash
(gdb)edit 3  # 编辑第三行
(gdb)edit func # 编辑func函数
(gdb)edit test.c:5 #编辑test.c第五行
复制代码
```

完了之后，重新编译程序( **注意一定要带上 shell 命令，表明是shell命令** )：

```css
(gdb)shell gcc -g -o main main.c test.c
复制代码
```

或者这样：

启动是带上 tui(Text User Interface)，可以在多个窗口调试：

```css
gdb main -tui
复制代码
```

### 1.6 带参数调试

**1. 启动的时候带上参数**

```css
gdb --args xxx 参数
复制代码
```

**2. 启动之后 run 带上参数**

```arduino
# gdb xxx
(gdb)run 参数
复制代码
```

**3. 启动之后 set args 设置参数**

```arduino
# gdb xxx
(gdb) set args 参数
复制代码
```

## 02 调试多进程

### 2.1 attach 方法

1. 首先找到需调试的子进程：`ps -ef | grep xxx` 或 `pidof` 进程名
2. 进入 `gdb` 模式，输入 `attach pid`
3. 打断点，运行进入调试

或者直接这样：`gdb <program> pid`（或 `gdb <program> --pid pid`），gdb 会 自动 attach。

如果出现如下错误：

```arduino
Could not attach to process.  If your uid matches the uid of the target
process, check the setting of /proc/sys/kernel/yama/ptrace_scope, or try
again as the root user.  For more details, see /etc/sysctl.d/10-ptrace.conf
ptrace: Operation not permitted.
复制代码
```

切换到 root 用户，将 `/etc/sysctl.d/10-ptrace.conf` 中的

```ini
kernel.yama.ptrace_scope = 1
复制代码
```

改为：

```ini
kernel.yama.ptrace_scope = 0
复制代码
```

### 2.2 follow-fork-mode mode 方法

1. 进入 `gdb` 模式，输入 `set follow-fork-mode mode` (mode 可选 parent、child，表示调试父进程还是子进程)
2. 打断点

### 2.3 调试已运行程序

已运行程序通常没有调试信息，但如果不能停止当前程序重新启动调试，可以：

同样的代码，再编译出一个带调试信息的版本，然后：

```arduino
# gdb
(gdb) file hello
Reading symbols from hello...done.
(gdb)attach 20829
复制代码
```

## 03 调试多线程

`gdb` 有一组命令能够辅助多线程的调试：

- `info threads`：显示当前可调式的所有线程，线程 ID 前有 “*” 表示当前被调试的线程。
- `thread id`：调试目标 id 指定的线程
- `set scheduler-locking [on|off|step]`：多线程环境下，会存在多个线程运行，这会影响调试某个线程的结果，这个命令可以设置调试的时候多个线程的运行情况，`on` 表示只有当前调试的线程会继续执行，`off` 表示不屏蔽任何线程，所有线程都可以执行，`step` 表示在单步执行时，只有当前线程会执行。

## 04 coredump 调试

coredump 调试依赖于 core 文件，core 文件是程序非法执行后 core dump 后产生的文件。这是 Linux 系统的一种保护机制，当出现某些连开发和测试费了九牛二虎之力都没能发现的问题时，Linux 系统还提供了最后一道屏障，通过 core 文件就可以让这些问题原形毕露。

### 4.1 开启 core dump

要想让程序崩溃时产生 core 文件，需要开启，输入 `ulimit -c`，如果输出为 0，表示默认关闭 core dump。

有两种方式可以开启，一种就是通过 ulimit 命令，一种是在程序中写代码开启，这里只讲第一种，第二种参考文末的[引用1](https://link.juejin.cn/?target=https%3A%2F%2Fwww.cnblogs.com%2FAnker%2Fp%2F6079580.html)。

```bash
ulimit -c unlimied  # 表示不限制core文件大小
ulimit -c 10        # 设置最大大小，单位为块，一块默认为512字节
复制代码
```

上面是临时开启，永久开启要修改 `/etc/security/limits.conf` 文件，增加一行：

```shell
# /etc/security/limits.conf
# <domain>        <type>  <item>  <value>
    *               soft    core    unlimited
复制代码
```

这样就可以生成 core 文件，文件名就是 core，并且默认在当前程序所在目录下生成，如果要指定目录，则可以 `echo "/tmp/corefile-%e-%p-%t" > /proc/sys/kernel/core_pattern` 设置 core 文件保存在目录 "/tmp/corefile" 下，文件名格式为 “core-命令名-pid-时间戳”

还可以通过 `echo 1 > /proc/sys/kernel/core_uses_pid` 使得生成的 core 文件变成 `core.pid`，pid 是该进程的 pid。

### 4.2 调试 core dump

使用

```xml
gdb <program> core文件名
复制代码
```

或者 gdb 启动后，使用

- `-core <file>`
- `-c <file>`

来调试 core 文件

下面是一个例子：

```arduino
#include <stdio.h>
int func(int *p)
{
    int y = *p;
    return y;
}
int main()
{
    int *p = NULL;
    return func(p);
}
复制代码
```

编译：`gdb -g -o core_dump core_dump.c`，用 gdb 查看 core 文件

```ruby
root@root:~$ gcc core_demo.c -o core_demo -g
root@root:~$ ./core_demo 
Segmentation fault (core dumped)

root@root:~$ gdb core_demo core_demo.core.24816
...
Core was generated by './core_demo'.
Program terminated with signal 11, Segmentation fault.
#0  0x080483cd in func (p=0x0) at core_demo.c:5
5       int y = *p;
(gdb)  where
#0  0x080483cd in func (p=0x0) at core_demo.c:5
#1  0x080483ef in main () at core_demo.c:12
(gdb) info frame
Stack level 0, frame at 0xffd590a4:
 eip = 0x80483cd in func (core_demo.c:5); saved eip 0x80483ef
 called by frame at 0xffd590c0
 source language c.
 Arglist at 0xffd5909c, args: p=0x0
 Locals at 0xffd5909c, Previous frame's sp is 0xffd590a4
 Saved registers:
  ebp at 0xffd5909c, eip at 0xffd590a0
(gdb) 
复制代码
```

可以看到，我们可以还原 core_demo 执行时的场景，并使用 where 查看当前程序调用函数栈帧，还可以使用 gdb 中的命令查看寄存器，变量等信息。

## 常见问题

### 问题 1

开启 GDB 调试时出现：

```perl
Missing separate debuginfos, use: debuginfo-install libgcc-4.8.5-28.el7_5.1.x86_64 numactl-libs-2.0.9-7.el7.x86_64
复制代码
```

**解决：**

1. 修改文件`/etc/yum.repos.d/CentOS-Debuginfo.repo`中的`enabled`参数，将其值修改为 1
2. `yum install nss-softokn-debuginfo --nogpgcheck`
3. `debuginfo-install glibc` 如果出现下面的问题： `-bash: debuginfo-install: command not found`，则先安装`yum-utils`，使用命令： `yum install yum-utils`
4. 分别安装问题提示的两个库：`use: debuginfo-install libgcc-4.8.5-28.el7_5.1.x86_64 numactl-libs-2.0.9-7.el7.x86_64`

## 参考

1 [linux下core dump总结](https://link.juejin.cn/?target=https%3A%2F%2Fwww.cnblogs.com%2FAnker%2Fp%2F6079580.html)

2 [Linux Core Dump](https://link.juejin.cn/?target=http%3A%2F%2Fwww.cnblogs.com%2Fhazir%2Fp%2Flinxu_core_dump.htm)

3 [GDB 调试利器](https://link.juejin.cn/?target=https%3A%2F%2Flinuxtools-rst.readthedocs.io%2Fzh_CN%2Flatest%2Ftool%2Fgdb.html)

