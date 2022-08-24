# MakeFile

[原文链接](https://gist.github.com/isaacs/62a2d1825d04437c6f08) 我会在我认为有价值的地方用中文写出

## makefile basics tutorial

````makefile
# Hello, and welcome to makefile basics.
#
# You will learn why `make` is so great, and why, despite its "weird" syntax,
# it is actually a highly expressive, efficient, and powerful way to build
# programs.
#
# Once you're done here, go to
# http://www.gnu.org/software/make/manual/make.html
# to learn SOOOO much more.

# To do stuff with make, you type `make` in a directory that has a file called
# "Makefile".  You can also type `make -f <makefile>` to use a different
# filename.
使用Make的方式 在有Makefile文件的时候，执行make命令
或者使用make -f <file> 指定需要执行的文件
#
# A Makefile is a collection of rules.  Each rule is a recipe to do a specific
# thing, sort of like a grunt task or an npm package.json script.
#
# A rule looks like this:
#
# <target>: <prerequisites...>
# 	<commands>
#
# The "target" is required.  The prerequisites are optional, and the commands
# are also optional, but you have to have one or the other.
目标是必须的，前置条件和指令需要有其中一个.
#
# Type "make" and see what happens:

tutorial:
	@# todo: have this actually run some kind of tutorial wizard?
	@echo "Please read the 'Makefile' file to go through this tutorial"

# By default, the first target is run if you don't specify one.  So, in this
# dir, typing "make" is the same as typing "make tutorial"
通常来讲，如果你没有指定执行的目标，第一个目标会被默认执行
#
# By default, make prints out the command before it runs it, so you can see
# what it's doing.  This is a departure from the "success should be silent"
# UNIX dogma, but without that default, it'd be very difficult to see what
# build logs etc are actually doing.
make的在执行指令前会将指令打印出来
#
# To suppress the output, we've added @ signs before each line, above.
你可以在每个指令之前加上一个@，这会使指令不会被输出
#
# Each line of the command list is run as a separate invocation of the shell.
# So, if you set a variable, it won't be available in the next line! To see
# this in action, try running `make var-lost`
每个指令都在一个独立的shell中执行, 如果你定义一个变量，它并不会在下一行生效。
# 
var-lost:
	export foo=bar
	echo "foo=[$$foo]"

# Notice that we have to use a double-$ in the command line.  That is because
# each line of a makefile is parsed first using the makefile syntax, and THEN
# the result is passed to the shell.
我们在这里使用了2个$，因为make会先使用makefile语法解析命令，然后传递给shell
# Let's try running both of the commands in the *same* shell invocation, by
# escaping the \n character.  Run `make var-kept` and note the difference.
在后面的文章中，有如何解决这个问题的方案

var-kept:
	export foo=bar; \
	echo "foo=[$$foo]"

# Now let's try making something that depends on something else.  In this case,
# we're going to create a file called "result.txt" which depends on
# "source.txt".

result.txt: source.txt
	@echo "building result.txt from source.txt"
	cp source.txt result.txt

# When we type `make result.txt`, we get an error!
# $ make result.txt
# make: *** No rule to make target `source.txt', needed by `result.txt'.  Stop.
#
# The problem here is that we've told make to create result.txt from
# source.txt, but we haven't told it how to get source.txt, and the file is
# not in our tree right now.
#
# Un-comment the next ruleset to fix the problem.
#
#source.txt:
#	@echo "building source.txt"
#	echo "this is the source" > source.txt
#
# Run `make result.txt` and you'll see it first creates source.txt, and then
# copies it to result.txt.  Try running `make result.txt` again, and you'll see
# that nothing happens!  That's because the dependency, source.txt, hasn't
# changed, so there's no need to re-build result.txt.
如果依赖没有发生变化，那么命令不会被执行，如果修改source.txt那么result会被重新构建
#
# Run `touch source.txt`, or edit the file, and you'll see that
# `make result.txt` re-builds the file.
#
#
# Let's say that we were working on a project with 100 .c files, and each of
# those .c files we wanted to turn into a corresponding .o file, and then link
# all the .o files into a binary.  (This is effectively the same if you have
# 100 .styl files to turn into .css files, and then link together into a big
# single concatenated main.min.css file.)
#
# It would be SUPER TEDIOUS to create a rule for each one of those.  Luckily,
# make makes this easy for us.  We can create one generic rule that handles
# any files matching a specific pattern, and declare that we're going to
# transform it into the corresponding file of a different pattern.
#
# Within the ruleset, we can use some special syntax to refer to the input
# file and the output file.  Here are the special variables:
在规则集下，我们可以使用一些特殊的符号来引用输入和输出的文件

# $@  The file that is being made right now by this rule (aka the "target")
#     You can remember this because it's like the "$@" list in a
#     shell script.  @ is like a letter "a" for "arguments.
#     When you type "make foo", then "foo" is the argument.
#
# $<  The input file (that is, the first prerequisite in the list)
#     You can remember this becasue the < is like a file input
#     pipe in bash.  `head <foo.txt` is using the contents of
#     foo.txt as the input.  Also the < points INto the $
#
# $^  This is the list of ALL input files, not just the first one.
#     You can remember it because it's like $<, but turned up a notch.
#     If a file shows up more than once in the input list for some reason,
#     it's still only going to show one time in $^.
#
# $?  All the input files that are newer than the target
#     It's like a question. "Wait, why are you doing this?  What
#     files changed to make this necessary?"
#
# $$  A literal $ character inside of the rules section
#     More dollar signs equals more cash money equals dollar sign.
#
# $*  The "stem" part that matched in the rule definition's % bit
#     You can remember this because in make rules, % is like * on
#     the shell, so $* is telling you what matched the pattern.
#
# You can also use the special syntax $(@D) and $(@F) to refer to
# just the dir and file portions of $@, respectively.  $(<D) and
# $(<F) work the same way on the $< variable.  You can do the D/F
# trick on any variable that looks like a filename.
#
# There are a few other special variables, and we can define our own
# as well.  Most of the other special variables, you'll never use, so
# don't worry about them.
#
# So, our rule for result.txt could've been written like this
# instead:

result-using-var.txt: source.txt
	@echo "buildling result-using-var.txt using the $$< and $$@ vars"
	cp $< $@

# Let's say that we had 100 source files, that we want to convert
# into 100 result files.  Rather than list them out one by one in the
# makefile, we can use a bit of shell scripting to generate them, and
# save them in a variable.
#
# Note that make uses := for assignment instead of =
# I don't know why that is.  The sooner you accept that this isn't
# bash/sh, the better.
#
# Also, usually you'd use `$(wildcard src/*.txt)` instead, since
# probably the files would already exist in your project.  Since this
# is a tutorial, though we're going to generate them using make.
#
# This will execute the shell program to generate a list of files.
srcfiles := $(shell echo src/{00..99}.txt)

# How do we make a text file in the src dir?
# We define the filename using a "stem" with the % as a placeholder.
# What this means is "any file named src/*.txt", and it puts whatever
# matches the "%" bit into the $* variable.
src/%.txt:
	@# First things first, create the dir if it doesn't exist.
	@# Prepend with @ because srsly who cares about dir creation
	@[ -d src ] || mkdir src
	@# then, we just echo some data into the file
	@# The $* expands to the "stem" bit matched by %
	@# So, we get a bunch of files with numeric names, containing their number
	echo $* > $@

# Try running `make src/00.txt` and `make src/01.txt` now.


# To not have to run make for each file, we define a "phony" target that
# depends on all of the srcfiles, and has no other rules.  It's good
# practice to define your phony rules in a .PHONY declaration in the file.
# (See the .PHONY entry at the very bottom of this file.)
#
# Running `make source` will make ALL of the files in the src/ dir.  Before
# it can make any of them, it'll first make the src/ dir itself.  Then
# it'll copy the "stem" value (that is, the number in the filename matched
# by the %) into the file, like the rule says above.
#
# Try typing "make source" to make all this happen.
source: $(srcfiles)


# So, to make a dest file, let's copy a source file into its destination.
# Also, it has to create the destination folder first.
#
# The destination of any dest/*.txt file is the src/*.txt file with
# the matching stem.  You could just as easily say that %.css depends
# on %.styl
dest/%.txt: src/%.txt
	@[ -d dest ] || mkdir dest
	cp $< $@

# So, this is great and all, but we don't want to type `make dest/#.txt`
# 100 times!
#
# Let's create a "phony" target that depends on all the destination files.
# We can use the built-in pattern substitution "patsubst" so we don't have
# to re-build the list.  This patsubst function uses the same "stem"
# concept explained above.

destfiles := $(patsubst src/%.txt,dest/%.txt,$(srcfiles))
destination: $(destfiles)

# Since "destination" isn't an actual filename, we define that as a .PHONY
# as well (see below).  This way, Make won't bother itself checking to see
# if the file named "destination" exists if we have something that depends
# on it later.
#
# Let's say that all of these dest files should be gathered up into a
# proper compiled program.  Since this is a tutorial, we'll use the
# venerable feline compiler called "cat", which is included in every
# posix system because cats are wonderful and a core part of UNIX.

kitty: $(destfiles)
	@# Remember, $< is the input file, but $^ is ALL the input files.
	@# Cat them into the kitty.
	cat $^ > kitty

# Note what's happening here:
#
# kitty -> (all of the dest files)
# Then, each destfile depends on a corresponding srcfile
#
# If you `make kitty` again, it'll say "kitty is up to date"
#
# NOW TIME FOR MAGIC!
#
# Let's update just ONE of the source files, and see what happens
#
# Run this:  touch src/25.txt; make kitty
#
# Note that it is smart enough to re-build JUST the single destfile that
# corresponds to the 25.txt file, and then concats them all to kitty.  It
# *doesn't* re-generate EVERY source file, and then EVERY dest file,
# every time


# It's good practice to have a `test` target, because people will come to
# your project, and if there's a Makefile, then they'll expect `make test`
# to do something.
#
# We can't test the kitty unless it exists, so we have to depend on that.
test: kitty
	@echo "miao" && echo "tests all pass!"

# Last but not least, `make clean` should always remove all of the stuff
# that your makefile created, so that we can remove bad stuff if anything
# gets corrupted or otherwise screwed up.
clean:
	rm -rf *.txt src dest kitty

# What happens if there's an error!?  Let's say you're building stuff, and
# one of the commands fails.  Make will abort and refuse to proceed if any
# of the commands exits with a non-zero error code.
# To demonstrate this, we'll use the `false` program, which just exits with
# a code of 1 and does nothing else.
badkitty:
	$(MAKE) kitty # The special var $(MAKE) means "the make currently in use"
	false # <-- this will fail
	echo "should not get here"

.PHONY: source destination clean test badkitty
````



[原文链接](http://www.ruanyifeng.com/blog/2015/02/make.html)

## 一、Make的概念

Make这个词，英语的意思是"制作"。Make命令直接用了这个意思，就是要做出某个文件。比如，要做出文件a.txt，就可以执行下面的命令。

> ```bash
> $ make a.txt
> ```

但是，如果你真的输入这条命令，它并不会起作用。因为Make命令本身并不知道，如何做出a.txt，需要有人告诉它，如何调用其他命令完成这个目标。

比如，假设文件 a.txt 依赖于 b.txt 和 c.txt ，是后面两个文件连接（cat命令）的产物。那么，make 需要知道下面的规则。

> ```bash
> a.txt: b.txt c.txt
>     cat b.txt c.txt > a.txt
> ```

也就是说，make a.txt 这条命令的背后，实际上分成两步：第一步，确认 b.txt 和 c.txt 必须已经存在，第二步使用 cat 命令 将这个两个文件合并，输出为新文件。

像这样的规则，都写在一个叫做Makefile的文件中，Make命令依赖这个文件进行构建。Makefile文件也可以写为makefile， 或者用命令行参数指定为其他文件名。

> ```bash
> $ make -f rules.txt
> # 或者
> $ make --file=rules.txt
> ```

上面代码指定make命令依据rules.txt文件中的规则，进行构建。

总之，make只是一个根据指定的Shell命令进行构建的工具。它的规则很简单，你规定要构建哪个文件、它依赖哪些源文件，当那些文件有变动时，如何重新构建它。

## 二、Makefile文件的格式

构建规则都写在Makefile文件里面，要学会如何Make命令，就必须学会如何编写Makefile文件。

### 2.1 概述

Makefile文件由一系列规则（rules）构成。每条规则的形式如下。

> ```bash
> <target> : <prerequisites> 
> [tab]  <commands>
> ```

上面第一行冒号前面的部分，叫做"目标"（target），冒号后面的部分叫做"前置条件"（prerequisites）；第二行必须由一个tab键起首，后面跟着"命令"（commands）。

"目标"是必需的，不可省略；"前置条件"和"命令"都是可选的，但是两者之中必须至少存在一个。

每条规则就明确两件事：构建目标的前置条件是什么，以及如何构建。下面就详细讲解，每条规则的这三个组成部分。

### 2.2 目标（target）

一个目标（target）就构成一条规则。目标通常是文件名，指明Make命令所要构建的对象，比如上文的 a.txt 。目标可以是一个文件名，也可以是多个文件名，之间用空格分隔。

除了文件名，目标还可以是某个操作的名字，这称为"伪目标"（phony target）。

> ```bash
> clean:
>       rm *.o
> ```

上面代码的目标是clean，它不是文件名，而是一个操作的名字，属于"伪目标 "，作用是删除对象文件。

> ```bash
> $ make  clean
> ```

但是，如果当前目录中，正好有一个文件叫做clean，那么这个命令不会执行。因为Make发现clean文件已经存在，就认为没有必要重新构建了，就不会执行指定的rm命令。

为了避免这种情况，可以明确声明clean是"伪目标"，写法如下。

> ```bash
> .PHONY: clean
> clean:
>         rm *.o temp
> ```

声明clean是"伪目标"之后，make就不会去检查是否存在一个叫做clean的文件，而是每次运行都执行对应的命令。像.PHONY这样的内置目标名还有不少，可以查看[手册](https://www.gnu.org/software/make/manual/html_node/Special-Targets.html#Special-Targets)。

如果Make命令运行时没有指定目标，默认会执行Makefile文件的第一个目标。

> ```bash
> $ make
> ```

上面代码执行Makefile文件的第一个目标。

### 2.3 前置条件（prerequisites）

前置条件通常是一组文件名，之间用空格分隔。它指定了"目标"是否重新构建的判断标准：只要有一个前置文件不存在，或者有过更新（前置文件的last-modification时间戳比目标的时间戳新），"目标"就需要重新构建。

> ```bash
> result.txt: source.txt
>     cp source.txt result.txt
> ```

上面代码中，构建 result.txt 的前置条件是 source.txt 。如果当前目录中，source.txt 已经存在，那么`make result.txt`可以正常运行，否则必须再写一条规则，来生成 source.txt 。

> ```bash
> source.txt:
>     echo "this is the source" > source.txt
> ```

上面代码中，source.txt后面没有前置条件，就意味着它跟其他文件都无关，只要这个文件还不存在，每次调用`make source.txt`，它都会生成。

> ```bash
> $ make result.txt
> $ make result.txt
> ```

上面命令连续执行两次`make result.txt`。第一次执行会先新建 source.txt，然后再新建 result.txt。第二次执行，Make发现 source.txt 没有变动（时间戳晚于 result.txt），就不会执行任何操作，result.txt 也不会重新生成。

如果需要生成多个文件，往往采用下面的写法。

> ```bash
> source: file1 file2 file3
> ```

上面代码中，source 是一个伪目标，只有三个前置文件，没有任何对应的命令。

> ```bash
> $ make source
> ```

执行`make source`命令后，就会一次性生成 file1，file2，file3 三个文件。这比下面的写法要方便很多。

> ```bash
> $ make file1
> $ make file2
> $ make file3
> ```

### 2.4 命令（commands）

命令（commands）表示如何更新目标文件，由一行或多行的Shell命令组成。它是构建"目标"的具体指令，它的运行结果通常就是生成目标文件。

每行命令之前必须有一个tab键。如果想用其他键，可以用内置变量.RECIPEPREFIX声明。

> ```bash
> .RECIPEPREFIX = >
> all:
> > echo Hello, world
> ```

上面代码用.RECIPEPREFIX指定，大于号（>）替代tab键。所以，每一行命令的起首变成了大于号，而不是tab键。

需要注意的是，每行命令在一个单独的shell中执行。这些Shell之间没有继承关系。

> ```bash
> var-lost:
>     export foo=bar
>     echo "foo=[$$foo]"
> ```

上面代码执行后（`make var-lost`），取不到foo的值。因为两行命令在两个不同的进程执行。一个解决办法是将两行命令写在一行，中间用分号分隔。

> ```bash
> var-kept:
>     export foo=bar; echo "foo=[$$foo]"
> ```

另一个解决办法是在换行符前加反斜杠转义。

> ```bash
> var-kept:
>     export foo=bar; \
>     echo "foo=[$$foo]"
> ```

最后一个方法是加上`.ONESHELL:`命令。

> ```bash
> .ONESHELL:
> var-kept:
>     export foo=bar; 
>     echo "foo=[$$foo]"
> ```

## 三、Makefile文件的语法

### 3.1 注释

井号（#）在Makefile中表示注释。

> ```bash
> # 这是注释
> result.txt: source.txt
>     # 这是注释
>     cp source.txt result.txt # 这也是注释
> ```

### 3.2 回声（echoing）

正常情况下，make会打印每条命令，然后再执行，这就叫做回声（echoing）。

> ```bash
> test:
>     # 这是测试
> ```

执行上面的规则，会得到下面的结果。

> ```bash
> $ make test
> # 这是测试
> ```

在命令的前面加上@，就可以关闭回声。

> ```bash
> test:
>     @# 这是测试
> ```

现在再执行`make test`，就不会有任何输出。

由于在构建过程中，需要了解当前在执行哪条命令，所以通常只在注释和纯显示的echo命令前面加上@。

> ```bash
> test:
>     @# 这是测试
>     @echo TODO
> ```

### 3.3 通配符

通配符（wildcard）用来指定一组符合条件的文件名。Makefile 的通配符与 Bash 一致，主要有星号（*）、问号（？）和 [...] 。比如， *.o 表示所有后缀名为o的文件。

> ```bash
> clean:
>         rm -f *.o
> ```

### 3.4 模式匹配

Make命令允许对文件名，进行类似正则运算的匹配，主要用到的匹配符是%。比如，假定当前目录下有 f1.c 和 f2.c 两个源码文件，需要将它们编译为对应的对象文件。

> ```bash
> %.o: %.c
> ```

等同于下面的写法。

> ```bash
> f1.o: f1.c
> f2.o: f2.c
> ```

使用匹配符%，可以将大量同类型的文件，只用一条规则就完成构建。

### 3.5 变量和赋值符

Makefile 允许使用等号自定义变量。

> ```bash
> txt = Hello World
> test:
>     @echo $(txt)
> ```

上面代码中，变量 txt 等于 Hello World。调用时，变量需要放在 $( ) 之中。

调用Shell变量，需要在美元符号前，再加一个美元符号，这是因为Make命令会对美元符号转义。

> ```bash
> test:
>     @echo $$HOME
> ```

有时，变量的值可能指向另一个变量。

> ```bash
> v1 = $(v2)
> ```

上面代码中，变量 v1 的值是另一个变量 v2。这时会产生一个问题，v1 的值到底在定义时扩展（静态扩展），还是在运行时扩展（动态扩展）？如果 v2 的值是动态的，这两种扩展方式的结果可能会差异很大。

为了解决类似问题，Makefile一共提供了四个赋值运算符 （=、:=、？=、+=），它们的区别请看[StackOverflow](https://stackoverflow.com/questions/448910/makefile-variable-assignment)。

> ```bash
> VARIABLE = value
> # 在执行时扩展，允许递归扩展。
> 
> VARIABLE := value
> # 在定义时扩展。
> 
> VARIABLE ?= value
> # 只有在该变量为空时才设置值。
> 
> VARIABLE += value
> # 将值追加到变量的尾端。
> ```

### 3.6 内置变量（Implicit Variables）

Make命令提供一系列内置变量，比如，$(CC) 指向当前使用的编译器，$(MAKE) 指向当前使用的Make工具。这主要是为了跨平台的兼容性，详细的内置变量清单见[手册](https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html)。

> ```bash
> output:
>     $(CC) -o output input.c
> ```

### 3.7 自动变量（Automatic Variables）

Make命令还提供一些自动变量，它们的值与当前规则有关。主要有以下几个。

**（1）$@**

$@指代当前目标，就是Make命令当前构建的那个目标。比如，`make foo`的 $@ 就指代foo。

> ```bash
> a.txt b.txt: 
>     touch $@
> ```

等同于下面的写法。

> ```bash
> a.txt:
>     touch a.txt
> b.txt:
>     touch b.txt
> ```

**（2）$<**

$< 指代第一个前置条件。比如，规则为 t: p1 p2，那么$< 就指代p1。

> ```bash
> a.txt: b.txt c.txt
>     cp $< $@ 
> ```

等同于下面的写法。

> ```bash
> a.txt: b.txt c.txt
>     cp b.txt a.txt 
> ```

**（3）$?**

$? 指代比目标更新的所有前置条件，之间以空格分隔。比如，规则为 t: p1 p2，其中 p2 的时间戳比 t 新，$?就指代p2。

**（4）$^**

$^ 指代所有前置条件，之间以空格分隔。比如，规则为 t: p1 p2，那么 $^ 就指代 p1 p2 。

**（5）$\***

$* 指代匹配符 % 匹配的部分， 比如% 匹配 f1.txt 中的f1 ，$* 就表示 f1。

**（6）$(@D) 和 $(@F)**

$(@D) 和 $(@F) 分别指向 $@ 的目录名和文件名。比如，$@是 src/input.c，那么$(@D) 的值为 src ，$(@F) 的值为 input.c。

**（7）$(<D) 和 $(<F)**

$(<D) 和 $(<F) 分别指向 $< 的目录名和文件名。

所有的自动变量清单，请看[手册](https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html)。下面是自动变量的一个例子。

> ```bash
> dest/%.txt: src/%.txt
>     @[ -d dest ] || mkdir dest
>     cp $< $@
> ```

上面代码将 src 目录下的 txt 文件，拷贝到 dest 目录下。首先判断 dest 目录是否存在，如果不存在就新建，然后，$< 指代前置文件（src/%.txt）， $@ 指代目标文件（dest/%.txt）。

### 3.8 判断和循环

Makefile使用 Bash 语法，完成判断和循环。

> ```bash
> ifeq ($(CC),gcc)
>   libs=$(libs_for_gcc)
> else
>   libs=$(normal_libs)
> endif
> ```

上面代码判断当前编译器是否 gcc ，然后指定不同的库文件。

> ```bash
> LIST = one two three
> all:
>     for i in $(LIST); do \
>         echo $$i; \
>     done
> 
> # 等同于
> 
> all:
>     for i in one two three; do \
>         echo $i; \
>     done
> ```

上面代码的运行结果。

> ```bash
> one
> two
> three
> ```

### 3.9 函数

Makefile 还可以使用函数，格式如下。

> ```bash
> $(function arguments)
> # 或者
> ${function arguments}
> ```

Makefile提供了许多[内置函数](https://www.gnu.org/software/make/manual/html_node/Functions.html)，可供调用。下面是几个常用的内置函数。

**（1）shell 函数**

shell 函数用来执行 shell 命令

> ```bash
> srcfiles := $(shell echo src/{00..99}.txt)
> ```

**（2）wildcard 函数**

wildcard 函数用来在 Makefile 中，替换 Bash 的通配符。

> ```bash
> srcfiles := $(wildcard src/*.txt)
> ```

**（3）subst 函数**

subst 函数用来文本替换，格式如下。

> ```bash
> $(subst from,to,text)
> ```

下面的例子将字符串"feet on the street"替换成"fEEt on the strEEt"。

> ```bash
> $(subst ee,EE,feet on the street)
> ```

下面是一个稍微复杂的例子。

> ```bash
> comma:= ,
> empty:=
> # space变量用两个空变量作为标识符，当中是一个空格
> space:= $(empty) $(empty)
> foo:= a b c
> bar:= $(subst $(space),$(comma),$(foo))
> # bar is now `a,b,c'.
> ```

**（4）patsubst函数**

patsubst 函数用于模式匹配的替换，格式如下。

> ```bash
> $(patsubst pattern,replacement,text)
> ```

下面的例子将文件名"x.c.c bar.c"，替换成"x.c.o bar.o"。

> ```bash
> $(patsubst %.c,%.o,x.c.c bar.c)
> ```

**（5）替换后缀名**

替换后缀名函数的写法是：变量名 + 冒号 + 后缀名替换规则。它实际上patsubst函数的一种简写形式。

> ```bash
> min: $(OUTPUT:.js=.min.js)
> ```

上面代码的意思是，将变量OUTPUT中的后缀名 .js 全部替换成 .min.js 。

## 四、Makefile 的实例

**（1）执行多个目标**

> ```bash
> .PHONY: cleanall cleanobj cleandiff
> 
> cleanall : cleanobj cleandiff
>         rm program
> 
> cleanobj :
>         rm *.o
> 
> cleandiff :
>         rm *.diff
> ```

上面代码可以调用不同目标，删除不同后缀名的文件，也可以调用一个目标（cleanall），删除所有指定类型的文件。

**（2）编译C语言项目**

> ```bash
> edit : main.o kbd.o command.o display.o 
>     cc -o edit main.o kbd.o command.o display.o
> 
> main.o : main.c defs.h
>     cc -c main.c
> kbd.o : kbd.c defs.h command.h
>     cc -c kbd.c
> command.o : command.c defs.h command.h
>     cc -c command.c
> display.o : display.c defs.h
>     cc -c display.c
> 
> clean :
>      rm edit main.o kbd.o command.o display.o
> 
> .PHONY: edit clean
> ```

今天，Make命令的介绍就到这里。下一篇文章我会介绍，[如何用 Make 来构建 Node.js 项目](https://www.ruanyifeng.com/blog/2015/03/build-website-with-make.html)。