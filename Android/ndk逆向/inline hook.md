
# inline assembly

示例:
``` C++
int add(int a, int b){
    int res;
    __asm("add %0, %1, %[b]" : // 汇编 0,1 是编号，b是变量别名
    "=r"(res) :                 // 输出寄存器 = 表示可以写入
    "r"(a), [b]"r"(b));         // 输入寄存器
    return res;
}
```

# inline hook

![inline hook 流程](<pic/inline hook/image.png>)