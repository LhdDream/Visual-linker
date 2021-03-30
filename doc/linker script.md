**一.概述**

- 链接过程由链接脚本(lds文件)控制,链接脚本主要用于规定输出文件的内存布局。(可以看做链接过程中的Makefile)
- 链接器的默认链接脚本,可以通过 ld --verbose 来进行查看，可以通过  -T 选项用于指定自己的链接脚本。

**二.简单例子**

```bash
SECTIONS
{
. = 0x10000;
.text : { *(.text) }
. = 0x800000;
.data : { *(.data) }
.bss : { *(.bss) }
}
```
- . = 0x10000 : 把定位器符号置为 0x10000 (若不指定, 则该符号的初始值为 0).
- .text : { *(.text) } : 将所有(*符号代表任意输入文件)输入文件  的.text section 合并成一个.text section, 该 section的地址由定位器符号的值指定, 即 0x10000.
- . = 0x800000 ：把定位器符号置为 0x800000
.data : { *(.data) } : 将所有输入文件的.text section合并成一个.data section, 该section的地址被置为0x800000.
- .bss : { *(.bss) } : 将所有输入文件的.bss section 合并成一个.bss section，该 section 的地址被置为0x800000+.data section 的大小.

**三.常用简单命令**

>ENTRY(SYMBOL) : 将符号 SYMBOL 的值设置成入口地址。

 入口地址(entry point): 进程执行的第一条用户空间的指令在进程地址空间的地址)

ld 有多种方法设置进程入口地址, 按一下顺序: (编号越前, 优先级越高)

 - ld 命令行的-e 选项
 - 连接脚本的 ENTRY(SYMBOL)命令
 - 如果定义了 start 符号, 使用 start 符号值
-  如果存在.text section, 使用.text section 的第一字节的位置值

 > SEARCH_DIR(PATH) ：定义搜索路径

- 定义从哪些文件夹中搜索，不过 ld -L的优先级更高

>STARTUP(filename) : 指定 filename 为第一个输入文件

- 在链接过程中, 每个输入文件是有顺序的. 此命令设置文件 filename 为第一个输入文件。

>PROVIDE 关键字
- 定义这类符号: 在目标文件内被引用，但没有在任何目标文件内被定义的符号

SECTIONS
{
.text :
{
*(.text)
_etext = .;
PROVIDE(etext = .);
}
}

- 当目标文件内引用了 etext 符号，确没有定义它时，etext 符号对应的地址被定义为.text section 之后的第一个字节的地址

> KEEP 命令

- 保留节内容，不被gc-section 去进行优化

>SECTIONS 命令

- SECTIONS 命令告诉ld 如何把输入文件的sections 映射到输出文件的各个section

SECTION [ADDRESS] [(TYPE)] : [AT(LMA)]
{
OUTPUT-SECTION-COMMAND
OUTPUT-SECTION-COMMAND
...
} [>REGION] [AT>LMA_REGION] [:PHDR :PHDR ...] [=FILLEXP]

[ ]内的内容为可选选项, 一般不需要.
SECTION：section 名字
SECTION 左右的空白、圆括号、冒号是必须的，换行符和其他空格是可选的。
每个 OUTPUT-SECTION-COMMAND 为以下四种之一，
符号赋值语句
一个输入 section 描述
直接包含的数据值
一个特殊的输出 section 关键字

-  REGION 属性显示地将该输出 section 限定于某块存储区域，当存储区域大小不能满足要求时，连接器会
报告该错误。