
# api hook

## 作用

在程序运行时，拦截函数调用，并执行自定义代码。在不修改现有代码的情况下为程序添加新功能。

## 实现方式

### 1. 替换函数入口

将函数入口处的指令替换为跳转到自定义代码的指令。

### 2. 写入自定义代码

在程序空白区域写入自定义代码，在自定义代码返回时执行原始代码。如果在自定义代码中调用原始函数，则在参数入栈后，`call`原始代码，在原始函数调用结束后，返回到自定义代码继续执行。

# 示例

使用DLL注入的方式实现，在`winmine.exe`中拦截`ShellAbout`函数。

```asm
.686
.model flat, stdcall
option casemap:none

include windows.inc
include kernel32.inc
include user32.inc
include shell32.inc

includelib Kernel32.lib
includelib user32.lib
includelib shell32.lib

Ori_Rroc proto

.data
    g_szShell32 db "Shell32", 0
    g_szShellAboutW db "ShellAboutW", 0
    g_ddJmpBackpoint dd 0 ; 原始代码返回地址
    g_szNewDlg dw 'h', 'o', 'o', 'k', 0
    g_szMyDlg dw 'm', 'y', 'a', 'b', 'o', 'u', 't', 0
    g_szAbout dw 'a', 'b', 'o', 'u', 't', 0

.code

ORI_CODE: ; 原始代码
    mov edi,edi
    push ebp
    mov ebp,esp
    jmp g_ddJmpBackpoint

HOOK_CODE: ; 自定义代码
    ; 替换参数
    mov dword ptr [esp + 8], offset g_szNewDlg
    
    ; 调用原始函数
    invoke Ori_Rroc

    ; 调用原始代码
    jmp ORI_CODE

Ori_Rroc proc
    push NULL
    push offset g_szAbout
    push offset g_szMyDlg
    push NULL
    call ORI_CODE
    
    ret

Ori_Rroc endp

InstallHook proc
    LOCAL hShell32:HMODULE
    LOCAL pfnShellAboutW:DWORD
    LOCAL dwOldProc:DWORD
    
    ; 获取要hook的api的地址
    invoke GetModuleHandle, offset g_szShell32
    mov hShell32, eax
    
    invoke GetProcAddress, hShell32, offset g_szShellAboutW
    mov pfnShellAboutW, eax
    
    ; 修改api头为jmp HOOKCODE
    invoke VirtualProtect, pfnShellAboutW, 5, PAGE_EXECUTE_READWRITE, addr dwOldProc ; 修改内存属性
    
    mov eax, pfnShellAboutW
    mov byte ptr [eax], 0e9h
    
    mov ecx, offset HOOK_CODE
    sub ecx, eax
    sub ecx, 5
    mov dword ptr [eax + 1], ecx
    
    add eax, 5
    mov g_ddJmpBackpoint, eax
    
    ret

InstallHook endp


DllMain proc hinstDLL:HINSTANCE, fdwReason:DWORD, lpvReserved:LPVOID 
    .if fdwReason == DLL_PROCESS_ATTACH
        invoke InstallHook
    .endif
    
    mov eax, TRUE
    ret
DllMain endp
    end DllMain
```