
# Debug理论

调试器的实现是通过异常机制实现的。

CPU（检测异常）->操作系统（处理异常）->调试器（处理异常）-> 被调试的程序。

正常启动的程序，它的父进程是桌面；当程序被调试器启动时，它的父进程是调试器。由此可以检测程序的父进程是否是调试器，达到反调试的目的。

示例：

``` C/C++
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

BOOL isDebug()
{
    HANDLE         hProcessSnap = NULL;
    BOOL           bRet = FALSE;
    PROCESSENTRY32 pe32 = { 0 };

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return (FALSE);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hProcessSnap, &pe32))
    {
        BOOL          bGotModule = FALSE;
        MODULEENTRY32 me32 = { 0 };

        do
        {
            if (pe32.th32ProcessID == GetCurrentProcessId())
            {
                // 判断父进程是否是桌面
                if (35772 != pe32.th32ParentProcessID) {
                    bRet = TRUE;
                    break;
                }
                else
                {
                    bRet = FALSE;
                    break;
                }
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
    return (bRet);
}

int main()
{
    if (isDebug())
    {
        printf("is Debug\n");
        system("pause");
        exit(0);
    }
    printf("is not Debug\n");
    system("pause");
	return 0;
}
```