#include "CWinThread.h"

IMPLEMENT_DYNCREATE(CWinThread, CCmdTarget)

CWinThread::CWinThread()
{
	
}

CWinThread::~CWinThread()
{
	
}

BOOL CWinThread::InitInstance()
{
	return 0;
}

int CWinThread::ExitInstance()
{
    return 0;
}

int CWinThread::Run()
{
    BOOL bRet;
    MSG msg;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            // handle the error and possibly exit
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	return bRet;
}
