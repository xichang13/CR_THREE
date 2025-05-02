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

MySub proto :dword, :dword

.data
    g_szClassName db "WindowsClass", 0
    g_szTitle db "Windows Title", 0
    fmt db "Date: %04d-%02d-%02d Time: %02d:%02d:%02d", 0
    g_szBuf db 100 dup(0)
.code

; WndProc 过程函数
WndProc proc hwnd:HWND, uMsg:DWORD, wParam:WPARAM, lParam:LPARAM
    local hdc:HDC
    local ps:PAINTSTRUCT
    local time:SYSTEMTIME
    local rc:RECT

    invoke RtlZeroMemory, addr rc, sizeof rc
    invoke RtlZeroMemory, addr time, sizeof time
    invoke RtlZeroMemory, addr ps, sizeof ps

    mov eax, 1000
    mov rc.right, eax
    mov eax, 100
    mov rc.top, eax
    mov eax, 200
    mov rc.bottom, eax

    .IF uMsg == WM_PAINT
        invoke BeginPaint, hwnd, addr ps
        mov hdc, eax
        invoke GetLocalTime, addr time
        mov ax, time.wSecond
        movzx eax, ax
        push eax
        mov ax, time.wMinute
        movzx eax, ax
        push eax
        mov ax, time.wHour
        movzx eax, ax
        push eax
        mov ax, time.wDay
        movzx eax, ax
        push eax
        mov ax, time.wMonth
        movzx eax, ax
        push eax
        mov ax, time.wYear
        movzx eax, ax
        push eax
        invoke wsprintf, addr g_szBuf, addr fmt
        invoke DrawText, hdc, addr g_szBuf, offset g_szBuf - offset fmt - 11, addr rc, DT_CENTER
    .ELSEIF uMsg == WM_CREATE
        invoke SetTimer, hwnd, 1, 100, NULL
    .ELSEIF uMsg == WM_TIMER
        invoke InvalidateRect, hwnd, NULL, TRUE
    .ELSEIF uMsg == WM_DESTROY
        invoke PostQuitMessage, hwnd
    .ELSE
        invoke DefWindowProc, hwnd, uMsg, wParam, lParam
    .ENDIF

    ret
WndProc endp

; WinMain
WinMain proc hInstance:HINSTANCE
    local wc:WNDCLASS
    local msg:MSG
    
    ; 注册窗口类
    ; 初始化
    invoke RtlZeroMemory, addr wc, sizeof wc
    mov wc.style, CS_HREDRAW or CS_VREDRAW
    mov wc.lpfnWndProc, offset WndProc
    mov eax, hInstance
    mov wc.hInstance, eax
    invoke LoadIcon, NULL, IDI_APPLICATION
    mov wc.hIcon, eax
    invoke LoadCursor, NULL, IDC_ARROW
    mov wc.hCursor, eax
    ; 使用资源
    mov wc.lpszMenuName, IDC_PROJECT2
    mov wc.hbrBackground, COLOR_WINDOW + 1
    mov wc.lpszClassName, offset g_szClassName
    invoke RegisterClass, addr wc
    .IF !eax
        ret
    .ENDIF

    ; 创建窗口
    invoke CreateWindowEx, 0, offset g_szClassName, offset g_szTitle, WS_OVERLAPPEDWINDOW,\
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL
    
    ; 显示窗口
    invoke ShowWindow, eax, SW_SHOW
    ; 更新窗口
    invoke UpdateWindow, eax

    ; 消息循环
    .WHILE 1
        invoke GetMessage, addr msg, NULL, 0, 0
        .BREAK .IF eax == 0
        invoke DispatchMessage, addr msg
    .endw

    ret
WinMain endp

entry:
    mov ax, 1
    mov bx, 2
    invoke MySub, ax, bx 

    invoke GetModuleHandle, NULL
    invoke WinMain, eax
    invoke ExitProcess, 0
end entry