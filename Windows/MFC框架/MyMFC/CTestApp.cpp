#include "CTestApp.h"
#include "CSingleDocTemplate.h"
#include "resource.h"
#include "CTestDoc.h"
#include "CTestFrame.h"
#include "CTestView.h"

IMPLEMENT_DYNCREATE(CTestApp, CWinApp)

CTestApp::CTestApp()
{
	
}

CTestApp::~CTestApp()
{
	
}

BOOL CTestApp::InitInstance()
{
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MENU1,
		RUNTIME_CLASS(CTestDoc),
		RUNTIME_CLASS(CTestFrame),       // Ö÷ SDI ¿ò¼Ü´°¿Ú
		RUNTIME_CLASS(CTestView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	CWinApp::OnFileNew();

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CTestApp::ExitInstance()
{
	return CWinThread::ExitInstance();
}

