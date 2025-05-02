#include "CTestApp.h"

thread_local CTestApp theApp;

CWinThread* AfxGetThread()
{
	return &theApp;
}

CWinApp* AfxGetApp()
{
	return &theApp;
}

HINSTANCE AfxGetInstanceHandle() {
	return AfxGetApp()->m_hInstance;
}

int AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	_In_ LPTSTR lpCmdLine, int nCmdShow) {
	int nReturnCode = -1;

	CWinThread* pThread = AfxGetThread();
	CWinApp* pApp = AfxGetApp();

	pApp->m_hInstance = hInstance;
	pApp->m_lpCmdLine = lpCmdLine;
	pApp->m_nCmdShow = nCmdShow;

	if (!pThread->InitInstance())
	{
		if (pThread->m_pMainWnd != NULL)
		{
			printf("Warning: Destroying non-NULL m_pMainWnd\n");
			pThread->m_pMainWnd->DestroyWindow();
		}
		nReturnCode = pThread->ExitInstance();
		goto InitFailure;
	}

	nReturnCode = pThread->Run();
InitFailure:
	return nReturnCode;
}

int main() {
	return AfxWinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);
}