- [虚拟机保护](#虚拟机保护)

# 虚拟机保护

* 代码模拟CPU =》 虚拟机
  * 精简指令集 定长
  * 复杂指令集 变长

定义一套指令集 - 变长精简指令集

``` C
// MyVmp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <memory.h>
#include <malloc.h>

/*
008D1080 55                   push        ebp  
008D1081 8B EC                mov         ebp,esp  
008D1083 8B 45 08             mov         eax,dword ptr [ebp+8]  
008D1086 03 45 0C             add         eax,dword ptr [ebp+0Ch]  
008D1089 5D                   pop         ebp  
008D108A C3                   ret  

VM后的指令：

代码模拟CPU =》 虚拟机
精简指令集  定长
复杂指令集  变长

定义一套指令集
变长精简指令集

操作码  操作数1 操作数2 
op(bits)  op1(32bits)  op2(32bits)

虚拟机：
1）寄存器虚拟机
256个寄存器

2）堆栈虚拟机

寄存器编号
00 eax;
04 ecx;
08 edx;
0c ebx;
10 ebp;
14 esp;
18 esi;
1c edi;
20 eflags;
24 eip;

操作码
00   vPush

struct VMConext {
    int eax;
    int ecx;
    int edx;
    int ebx;
    ...
};
00 vPush Reg32
01 vPop  Reg32
02 vPush Imm8
03 vAdd 
04 vRead
05 vRet

真实环境  虚拟环境

寄存器轮转

vPush 0   eax
vPush 0   ebx

*/
struct VMConext {
    int eax;
    int ecx;
    int edx;
    int ebx;
    int ebp;
    int *esp;
    int esi;
    int edi;
    int eflags;
    unsigned char* opcode;
    int state;
};

unsigned char g_Op[] = {
    0x01, 0x00,     //vPop  eax
    0x01, 0x04,     //vPop  ecx
    0x01, 0x04,     //vPop  ecx


    0x00, 0x10,     //vPush ebp
    0x00, 0x14,     //vPush esp
    0x01, 0x10,     //vPop  ebp
    0x00, 0x10,     //vPush ebp
    0x02, 0x08,     //vPush 8               
    0x03,           //vAdd
    0x04,           //vRead
    0x01, 0x00,     //vPop  eax
    0x00, 0x10,     //vPush ebp
    0x02, 0x0c,     //vPush c               
    0x03,           //vAdd
    0x04,           //vRead
    0x00, 0x00,     //vPush eax
    0x03,           //vAdd
    0x01, 0x00,     //vPop  eax
    0x01, 0x10,     //vPop  ebp
    0x05,           //vRet
};

typedef int (*HANDLE)(VMConext* ctx);

const char* g_Regs[] = {
"eax",
"ecx",
"edx",
"ebx",
"ebp",
"esp",
"esi",
"edi",
"flg",
"eip",
};

void show_regs(VMConext* ctx) {
    for (int i = 0; i < 10; i++) {
        if (i != 0 && i % 4 == 0)
            printf("\n");
        printf("%s=%08x ", g_Regs[i], ((int*)ctx)[i]);
    }
    printf("\n");
}

int vPushReg32(VMConext* ctx) {
    unsigned char op = *ctx->opcode++;
    unsigned char op1 = *ctx->opcode++;
 
    printf("%02x %02x vPush %s\n", op, op1, g_Regs[op1 / 4]);
    ctx->esp--;
    *ctx->esp = *(int*)((char*)ctx + op1);
    //show_regs(ctx);

    //add   位运算
    return 0;
}

int vPopReg32(VMConext* ctx) {
    unsigned char op = *ctx->opcode++;
    unsigned char op1 = *ctx->opcode++;

    printf("%02x %02x vPop %s\n", op, op1, g_Regs[op1 / 4]);
    *(int*)((char*)ctx + op1) = *ctx->esp;
    ctx->esp++;
    //show_regs(ctx);
    return 0;
}

int vPushImm8(VMConext* ctx) {
    unsigned char op = *ctx->opcode++;
    unsigned char imm8 = *ctx->opcode++;

    printf("%02x %02x vPush %d\n", op, imm8, imm8);
    ctx->esp--;
    *ctx->esp = imm8;
    //show_regs(ctx);
    return 0;
}

int vAdd(VMConext* ctx) {
    unsigned char op = *ctx->opcode++;
    unsigned  op1 = *ctx->esp++;
    unsigned  op2 = *ctx->esp++;
    unsigned  ret = op1 + op2;
    ctx->esp--;
    *ctx->esp = ret;
    printf("%02x vAdd %d+%d=%d\n", op, op1, op2, ret);
    //show_regs(ctx);
    return 0;
}

int vRead(VMConext* ctx) {
    unsigned char op = *ctx->opcode++;
    unsigned  op1 = *ctx->esp++;
    unsigned  ret = *(int*)op1;
    ctx->esp--;
    *ctx->esp = ret;
    printf("%02x vRead [%08x]=%d\n", op, op1, ret);
    //show_regs(ctx);
    return 0;
}

int vRet(VMConext* ctx) {
    unsigned char op = *ctx->opcode++;
    printf("%02x vRet\n", op);
    ctx->state = -1;
    return 0;
}

HANDLE g_Handles[] = {
    &vPushReg32,  //xor key   add  
    &vPopReg32,  
    &vPushImm8,
    &vAdd,
    &vRead,
    &vRet,
};

int vm_init(VMConext *ctx, unsigned char* opcode) {
    memset(ctx, 0, sizeof(VMConext));  //更新真实寄存器环境到虚拟环境
    ctx->opcode = opcode;
    ctx->esp = (int*)malloc(0x1000);
    ctx->esp = (int*)((char*)ctx->esp + 0x1000); //sub esp, c0h   
    *ctx->esp-- = 2;
    *ctx->esp = 1;
   
    return 0;
}

int vm_entry(unsigned char *opcode) {
    //初始化虚拟机
    VMConext ctx;
    vm_init(&ctx, opcode);

    //平坦优化

    //虚拟机循环
    while (true) {
        if (ctx.state != 0)
            break;

        //1.取指令
        unsigned char op = *(ctx.opcode);

        //2.译码 3.执行
        g_Handles[op](&ctx);
    }

    return ctx.eax;
}

int add(int x, int y) {
    return vm_entry(g_Op);    
   /*
   */
}

int main()
{
    printf("1+2=%d", add(1, 2));
    return 0;
}
```