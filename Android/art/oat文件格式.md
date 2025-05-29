
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [OatFile::Open](#oatfileopen)
  - [`OpenOatFile`](#openoatfile)
    - [`ComputeFields`](#computefields)
    - [`Setup` 解析OatFile](#setup-解析oatfile)
- [回到class_linker.cc中 `LoadClassMembers`](#回到class_linkercc中-loadclassmembers)
  - [`FindOatClass`](#findoatclass)
    - [`GetOatClass`](#getoatclass)

<!-- /code_chunk_output -->

* 使用 `oatdump` 可以查看 oat文件信息

# OatFile::Open

> https://xrefandroid.com/android-8.1.0_r81/xref/art/runtime/oat_file.cc#Open

* `oat_filename` - 名字
* `oat_location` - 全路径
* `oat_file_begin` - 在内存中的起始地址
* `abs_dex_location` - dex文件的路径

## `OpenOatFile`

> https://xrefandroid.com/android-8.1.0_r81/xref/art/runtime/oat_file.cc#OpenOatFile

* 这里分别会调用`DlOpenOatFile::Load`和`ElfFile::Load`
    * `DlOpenOatFile` - 调用`dlopen`
    * `ElfFile` - 自己映射

### `ComputeFields`

> https://xrefandroid.com/android-8.1.0_r81/xref/art/runtime/oat_file.cc#ComputeFields

* 解析符号表
* 获取oat头在`*.odex`文件中的地址

### `Setup` 解析OatFile

> https://xrefandroid.com/android-8.1.0_r81/xref/art/runtime/oat_file.cc#Setup

* 读取`OatHeader`信息包含dex文件的数量
* 获取每个dex文件的偏移
* 解析dex文件头获取类偏移

# 回到class_linker.cc中 `LoadClassMembers`

## `FindOatClass`

> https://xrefandroid.com/android-8.1.0_r81/xref/art/runtime/oat_file.cc?fi=FindOatClass#FindOatClass

* 解析类中的方法（如果有oat文件）

### `GetOatClass`

> https://xrefandroid.com/android-8.1.0_r81/xref/art/runtime/oat_file.cc?fi=FindOatClass#GetOatClass

* 传入 `class_def_index` 拿到 `OatClass`