
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [smali指令集](#smali指令集)
  - [类型](#类型)
  - [寄存器](#寄存器)
  - [语句格式说明](#语句格式说明)
  - [数组操作](#数组操作)
  - [类定义](#类定义)
  - [对象操作](#对象操作)
  - [方法调用](#方法调用)

<!-- /code_chunk_output -->


# smali指令集

## 类型

* 方法 - `L全路径类名;->方法名(参数类型)返回类型`
* 字段 - `L全路径类名;->字段名:字段类型`

## 寄存器

* `smali`寄存器每个都是32位，编号从0到65535，名称为`v0`到`v65535`
* 每个函数开始都会使用`.registers n`指令来指定寄存器数量，`n`为寄存器数量
* `p0~pn-1` 参数寄存器
* `v0~vn-1` 局部寄存器

## 语句格式说明

[Dalvik 字节码格式](https://source.android.google.cn/docs/core/runtime/dalvik-bytecode?hl=zh-cn)

[Dalvik 可执行指令格式](https://source.android.google.cn/docs/core/runtime/instruction-formats?hl=zh-cn)

* 汇编语句格式 - `指令 目标寄存器,寄存器,...`
* 长度相同
    * 默认32位长度
    * 64位添加后缀`-wide`

## 数组操作

* `new-array vA, vB, type@CCCC` - 创建数组，`vA`为数组寄存器，`vB`为数组长度，`type@CCCC`为元素类型
* `filled-new-array {vC, vD, vE, vF, vG}, type@BBBB` - 初始化少于等于5个元素的数组
* `filled-new-array/range {vCCCC .. vNNNN}, type@BBBB` - 使用个数多于5个的寄存器初始化数组
* `fill-array-data vAA, +BBBBBBBB` - 使用数据表初始化数组
* `arrayop vAA, vBB, vCC` - 数组操作，`vAA`为数组寄存器，`vBB`为索引寄存器，`vCC`为元素寄存器
* `array-length vA, vB` - 获取数组长度，`vA`为数组寄存器，`vB`为长度寄存器

## 类定义

* 类
``` Java
.class <访问权限修饰符> [非权限修饰符] <类名>
.super <父类>
```
* 接口
``` Java
.implements Landroid/view/View$OnClickListener;
```
* 内部类和嵌套类 - 每个类都有一个`smali`文件，命名规则`外部类$内部类$内部类.smali`
* 字段
    * 普通字段 - `.field <访问权限修饰符> [非权限修饰符] <字段名>:<字段类型>`
    * 静态字段 - `.field <访问权限> static [修饰词] <字段名>:<字段类型>`
* 方法
``` Java
.method <访问权限修饰符> [非权限修饰符] <方法名>(<参数类型>...)<返回类型>
    .registers n
    <指令>
.end
```

## 对象操作

* `new-instance vAA, type@BBBB` - 创建对象
* `instance-of vA, vB, type@CCCC` - 判断对象类型
* `check-cast vAA, type@BBBB` - 强转对象类型

## 方法调用

* `invoke-kind {vC, vD, vE, vF, vG}, meth@BBBB` - 调用方法，参数少于5个
* `invoke-kind/range {vCCCC.. vNNNN}, meth@BBBB` - 调用方法，参数多于5个