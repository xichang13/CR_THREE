- [asm基础语法](#asm基础语法)
  - [环境配置](#环境配置)
  - [入口和段](#入口和段)
  - [常量](#常量)
  - [变量](#变量)
    - [整数](#整数)
    - [字符串](#字符串)
    - [数组](#数组)
    - [属性](#属性)
    - [堆栈](#堆栈)
  - [调用dos功能号](#调用dos功能号)
  - [功能号](#功能号)

# asm基础语法

## 环境配置

1. 拷贝masm611到指定目录
2. 将masm611目录添加到环境变量 set PATH=%PATH%;d:/masm
3. `ml /c *.asm` 编译
4. `link *.obj` 链接
5. `debug *.exe` 调试
6. `*.exe` 运行

## 入口和段

入口
* 入口点指定使用关键字 `end`，后面跟上程序的入口点名称
* 程序从入口点开始执行

段
* 一个程序必须至少有一个段
* 一个程序可以定义多个段
* 段不能嵌套
* 段可以重名，重名的段会被编译到同一块内存中

## 常量

整数
* 整数支持多个进制
* 数值必须以数字开头，如果为非数字，前面必须加0
* 负数前面可以加减号(-)

| 关键字 | 说明 | 示例 |
| --- | --- | --- |
| 无  | 默认为十进制 | mov ax, 12 |
| D   | 十进制 | mov ax, 12d |
| B   | 二进制 | mov ax, 101b |
| O   | 八进制 | mov ax, 12o |
| H   | 十六进制 | mov ax, 0C0h |

字符
* 字符必须用单引号括起来

mov byte ptr [bx], '$'

## 变量

### 整数

* 整数支持多个类型
* 整数可以有多个初值，未初始化的值用问号(?)表示
* 变量一般定义在一个单独的段中

```asm
; 变量名 类型  初始值
   val   dd   1234h
```

| 关键字 | 意义 |
| --- | --- |
| db | 字节 |
| dw | 字 |
| dd | 双字 |
| dq | 8字节 |
| dt | 10字节 |

### 字符串

* 字符串都可以用单引号(')或双引号(")
* 字符串一般以美元符($)结尾

```asm
g_sz db "hello world", 0dh, 0ah, '$'
```

### 数组

格式：
* 数组名 类型 值1, 值2, ..., 值n
* 数组名 类型 数量 dup(初值), ...

```asm
g_buf db 11h, 22h, 33h
g_buf2 db 10 dup(0)
```

### 属性

* masm提供了很多伪指令，可以获取变量的大小和地址，称之为变量的属性

| 关键字 | 意义 |
| --- | --- |
| seg | 获取段地址 |
| offset | 获取变量在段中的偏移地址 |
| type | 获取元素类型大小 |
| length | 获取元素个数 |
| size | 获取数据大小(length*type) |

### 堆栈

* stack 关键字让程序在被加载时指定 ss、bp和sp的值
* 使用数组为栈设置大小

``` asm
stack_seg segment stack
    db 256 dup(0cch)
stack_seg ends

data_seg segment
    g_a dd 1234h
    g_sz db "hello world", 0dh, 0ah, '$'
    g_buf db 10 dup(0)
    g_szFile db "test.txt", 0
    g_hFile dw ?
data_seg ends

code segment
start:
    ; 设置数据段
    mov ax, data_seg
    mov ds, ax
    
    ;打开文件
    lea dx, g_szFile
    mov cx, 0
    mov ah, 3ch
    int 21h 
    mov g_hFile, ax 

    ;写入文件
    mov bx, g_hFile
    mov cx, offset g_buf - offset g_sz
    lea dx, g_sz 
    mov ah, 40h 
    int 21h

    ;关闭文件
    mov bx, g_hFile
    mov ah, 3eh
    int 21h

    ; 访问变量，方法一
    mov ax, g_a
    mov al, g_sz
    mov ah, g_buf

    ; 访问变量，方法二
    lea bx, g_a
    mov al, [bx]

    ; 字符输入
    mov ah, 01
    int 21h

    ; 输出字符
    mov dl, al
    mov ah, 02
    int 21h

    ; 输出字符串
    lea dx, g_sz
    mov ah, 09h
    int 21h

    ; 调用中断
    mov ax, 4c00h
    int 21h

code ends

end start
```

## 调用dos功能号

## 功能号

* dos系统提供的功能(API)，通过21号中断来调用
* 每个功能都有一个编号，通过AH指定功能号

