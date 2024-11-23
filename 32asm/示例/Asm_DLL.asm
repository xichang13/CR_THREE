.386
.model flat, stdcall
option casemap:none

; 头文件
include windows.inc
include kernel32.inc
include user32.inc
include DisplayTime.inc

; 库文件
includelib User32.lib
includelib Kernel32.lib
includelib Dll1.lib

.data

.code
MyAdd proc n1:dword, n2:dword
    mov eax, n1
    add eax, n2
    ret
MyAdd endp

DllMain proc hModule:HMODULE, ul_reason_for_call:DWORD, lpReserved:LPVOID
    mov eax, TRUE
    ret
DllMain endp
end DllMain