
``` C++
BOOLEAN __stdcall MmIsAddressValid(PVOID VirtualAddress){
    // 项目录表索引
    DWORD dwPDTIndex = (VirtualAddress >> 20) & 0xFFC;
    // 页目录表项   0 - 0x3FD00000 ==> 0xC0300000
    DWORD dwPDPE = *(DWORD*)(dwPDTIndex + 0xC0300000);

    // 检查 P 位存在位
    if((dwPDPE & 1) == 0){
        // 无效地址
        return FALSE;
    }

    // 检查 PS 页大小 大页
    if((dwPDPE & 0x80) != 0){
        // 大页
        return TRUE;
    }

    // 
    DWORD dwPTIndex = (VirtualAddress >> 10) & 0x3FFFC;
    // 
    DWORD dwPDE = *(DWORD*)(dwPTIndex + 0xC0000000);

    // 检查 P 位存在位
    if((dwPDE & 1) == 0){
        // 无效地址
        return FALSE;
    }

    // 检查 G 全局标志位
    if((dwPDE & 0x80) == 0){
        // 局部页
        return TRUE;
    }

    // 页表索引
    dwPTIndex = dwPDE & 0xFFC;
    // 页表项
    dwPDE = *(DWORD*)(dwPTIndex + 0xC0300000);

    // 检查 P 和 PS 位
    if((dwPDE & 0x81) != 0){
        return TRUE;
    }
    return FALSE;
}
```