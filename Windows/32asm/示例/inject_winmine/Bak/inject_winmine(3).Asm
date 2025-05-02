.586
.model flat,stdcall
option casemap:none

   include windows.inc
   include user32.inc
   include kernel32.inc
   
   includelib user32.lib
   includelib kernel32.lib


WinMain proto :DWORD,:DWORD,:DWORD,:DWORD


.data?
   hInstance HINSTANCE ?
   CommandLine LPSTR ?
   
.data
    g_szWinmine db "扫雷", 0
    g_szUserLibrary db "user32", 0
    g_szMessageBoxA db "MessageBoxA", 0
.code

DATA_BEGIN: ; 写入的数据
    g_szTitle db "扫雷", 0
    g_szText db "你被注入了", 0
    g_pfnMsgBox dd 0 
CODE_BEGIN: ; 写入的代码
    ; 获取运行时代码位置
    call $ + 5
NEXT:
    pop ebx  ; 获取当前位置
    ; sub ebx, offset NEXT - offset DATA_BEGIN
    sub ebx, offset NEXT ; 获取偏移
    
    ; 调用MessageBox
    push MB_OK
    
    mov eax, offset g_szTitle
    add eax, ebx
    push eax
    
    mov eax, offset g_szText
    add eax, ebx
    push eax
    
    push NULL
    
    mov eax, offset g_pfnMsgBox
    add eax, ebx
    call dword ptr [eax]
    
    ret 4
CODE_END:
    

WinMain proc hInst:HINSTANCE,hPrevInst:HINSTANCE,CmdLine:LPSTR,CmdShow:DWORD
    LOCAL hWnd:HWND
    LOCAL dwPID:DWORD
    LOCAL hProc:HANDLE
    LOCAL pAddr:LPVOID
    LOCAL dwByteWrited:DWORD
    LOCAL hUser32:HINSTANCE
    LOCAL dwOld:DWORD
    
    ; 修改内存属性
    invoke VirtualProtect, offset DATA_BEGIN, 4, PAGE_EXECUTE_READWRITE, addr dwOld 
    
    ; 获取MessageBox的地址
    invoke LoadLibrary, offset g_szUserLibrary
    mov hUser32, eax
    invoke GetProcAddress, hUser32, offset g_szMessageBoxA
    mov g_pfnMsgBox, eax
    
    
    ; 打开进程
    invoke FindWindow, NULL, offset g_szWinmine
    mov hWnd, eax

    ; 获取进程ID
    invoke GetWindowThreadProcessId, hWnd, addr dwPID

    ; 打开进程，获取进程句柄
    invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, dwPID
    mov hProc, eax
    
    ; 申请内存，获取内存地址
    invoke VirtualAllocEx, hProc, NULL, 1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE
    mov pAddr, eax
    
    ; 写入代码
    invoke WriteProcessMemory, hProc, pAddr, offset DATA_BEGIN, offset WinMain - offset DATA_BEGIN, addr dwByteWrited
    
    ; 远程线程执行
    mov eax, pAddr
    add eax, offset CODE_BEGIN - offset DATA_BEGIN
    invoke CreateRemoteThread, hProc, NULL, 0, eax, NULL, 0, NULL
    
	ret
WinMain endp

start:
	invoke GetModuleHandle, NULL
	mov    hInstance,eax
	
	invoke GetCommandLine
	mov    CommandLine,eax
	
	invoke WinMain, hInstance,NULL,CommandLine, SW_SHOWDEFAULT
	invoke ExitProcess,eax


end start
