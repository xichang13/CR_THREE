
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [elf格式](#elf格式)
  - [数据类型](#数据类型)
  - [elf头](#elf头)
  - [节头](#节头)
  - [符号表](#符号表)
  - [程序头](#程序头)
    - [内存dump](#内存dump)
- [工具](#工具)

<!-- /code_chunk_output -->


# elf格式

## 数据类型

| 名称 | 大小 | 对齐值 | 用途 |
| --- | --- | --- | --- |
| Elf64_Addr | 8 | 8 | 无符号程序地址 |
| Elf64_Half | 2 | 2 | 无符号中整数 |
| Elf64_Off | 8 | 8 | 无符号文件偏移 |
| Elf64_Sword | 4 | 4 | 有符号整数 |
| Elf64_Word | 4 | 4 | 无符号整数 |
| Elf64_Xword | 8 | 8 | 无符号长整数 |
| Elf64_Sxword | 8 | 8 | 带符号长整数 |
| unsigned char | 1 | 1 | 无符号小整数 |

## elf头

``` C++
#define EI_NIDENT 16
typedef struct {
    unsigned char e_ident[EI_NIDENT];   // ELF文件标识符
    Elf32_Half e_type;                  // ELF文件类型
    Elf32_Half e_machine;               // ELF文件架构
    Elf32_Word e_version;               // ELF文件版本
    Elf32_Addr e_entry;                 // ELF文件入口地址
    Elf32_Off e_phoff;                  // ELF程序头表偏移量
    Elf32_Off e_shoff;                  // ELF节表偏移量
    Elf32_Word e_flags;                 // ELF标志位
    Elf32_Half e_ehsize;                // ELF文件头结构大小
    Elf32_Half e_phentsize;             // ELF程序头表结构大小
    Elf32_Half e_phnum;                 // ELF程序头表个数
    Elf32_Half e_shentsize;             // ELF节表结构大小
    Elf32_Half e_shnum;                 // ELF节表个数
    Elf32_Half e_shstrndx;              // 节表名字符串表的索引号
} Elf32_Ehdr;
typedef struct {
    unsigned char e_ident[EI_NIDENT];
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry;
    Elf64_Off e_phoff;
    Elf64_Off e_shoff;
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
} Elf64_Ehdr;
```

## 节头

``` C++
typedef struct {
    Elf64_Word sh_name;     // 节名表索引
    Elf64_Word sh_type;     // 节类型
    Elf64_Xword sh_flags;   // 节标志位
    Elf64_Addr sh_addr;     // 节在内存中的地址
    Elf64_Off sh_offset;    // 节在文件中的偏移量
    Elf64_Xword sh_size;    // 节大小
    Elf64_Word sh_link;     // 字符串表的索引
    Elf64_Word sh_info;     // 节附加信息
    Elf64_Xword sh_addralign;// 节地址对齐值
    Elf64_Xword sh_entsize; // 节元素大小
} Elf64_Shdr;
```

## 符号表

``` C++
typedef struct {
    Elf64_Word st_name;     // 在符号字符串表的索引
    unsigned char st_info;  // 符号类型和绑定属性
    unsigned char st_other; // 符号可见性
    Elf64_Half st_shndx;    // 名字字符串存在哪个节里面
    Elf64_Addr st_value;    // 在内存中的偏移
    Elf64_Xword st_size;    // 大小
} Elf64_Sym;
```

## 程序头

``` C++
typedef struct {
    Elf64_Word p_type;  // 类型
    Elf64_Word p_flags;
    Elf64_Off p_offset;
    Elf64_Addr p_vaddr;
    Elf64_Addr p_paddr;
    Elf64_Xword p_filesz;
    Elf64_Xword p_memsz;
    Elf64_Xword p_align;
} Elf64_Phdr;
```

* `p_type` - 接类型
    * `PT_LOAD` - 程序加载到内存中
    * `PT_DYNAMIC` - 动态链接表、字符串表等等
    * `PT_INTERP` - ELF程序链接器路径
    * `PT_PHDR` - 描述自身段头大小
* `p_flags` - 内存属性相关
    * `PF_X` - 可执行
    * `PF_R` - 只读
    * `PF_W` - 可写
* `p_offset` - 程序头表在文件中的偏移量
* `p_vaddr` - 程序加载到内存的地址
* `p_paddr` - 程序加载到内存的物理地址(PC和arm不用)
* `p_filesz` - 程序在文件中的大小
* `p_memsz` - 程序在内存中的大小
* `p_align` - 节地址对齐值

### 内存dump

从 /proc/pid/mem dump到文件
* 根据 PT_LOAD 节的内存地址，和文件大小拷贝到文件

# 工具

* llvm-readelf - 命令行查看elf文件信息
* llvm-nm - 查看elf符号信息
* xelf - 带界面的elf解析工具
* Imhex - 很像010的elf解析工具
