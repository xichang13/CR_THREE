
# Debug����

��������ʵ����ͨ���쳣����ʵ�ֵġ�

CPU������쳣��->����ϵͳ�������쳣��->�������������쳣��-> �����Եĳ���

���������ĳ������ĸ����������棻�����򱻵���������ʱ�����ĸ������ǵ��������ɴ˿��Լ�����ĸ������Ƿ��ǵ��������ﵽ�����Ե�Ŀ�ġ�

ʾ����

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
                // �жϸ������Ƿ�������
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