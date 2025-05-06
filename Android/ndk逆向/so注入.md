
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [so注入](#so注入)
  - [实现思路](#实现思路)
  - [跨进程函数调用](#跨进程函数调用)

<!-- /code_chunk_output -->


# so注入

## 实现思路

* 跨进程调用`mmap`，申请内存
* 跨进程写入so路径 - `process_vm_write`、`/proc`和`ptrace`
* 获取`dlopen`地址
    * 获取模块偏移
    * 获取模块基址
    * 基址 + 偏移
* 跨进程调用`dlopen`，加载so

## 跨进程函数调用

1. 获取函数在目标进程中的地址
2. 保存寄存器环境
3. 修改pc为函数地址，修改x30为0
4. 恢复目标进程运行
5. 等待信号
6. 恢复寄存器环境
