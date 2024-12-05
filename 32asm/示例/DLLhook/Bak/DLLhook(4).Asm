.686
.model flat,stdcall
option casemap:none

include windows.inc
include user32.inc
include kernel32.inc
include shell32.inc

includelib user32.lib
includelib kernel32.lib
includelib shell32.lib


.data
    g_szShell32 db "Shell32", 0
    g_szShellAboutW db "ShellAboutW", 0
    g_ddJmpBackpoint dd 0 ; 原始代码返回地址
    g_szNewDlg dw 'h', 'o', 'o', 'k', 0
    g_szMyDlg dw 'm', 'y', 'a', 'b', 'o', 'u', 't', 0
    g_szAbout dw 'a', 'b', 'o', 'u', 't', 0

Ori_Rroc proto

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