#include "CWinApp.h"
#include "CFrameWnd.h"

IMPLEMENT_DYNCREATE(CWinApp, CWinThread)

CWinApp::CWinApp()
{
	
}

CWinApp::~CWinApp()
{
	
}

void CWinApp::AddDocTemplate(CDocTemplate* pTemplate)
{
    if (m_pDocManager == NULL)
        m_pDocManager = new CDocManager;
    m_pDocManager->AddDocTemplate(pTemplate);
}

BOOL CWinApp::DoPromptFileName(std::string& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	return m_pDocManager->DoPromptFileName(fileName, nIDSTitle, lFlags,
		bOpenFileDialog, pTemplate);
}

CDocument* CWinApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	return m_pDocManager->OpenDocumentFile(lpszFileName);
}

void CWinApp::OnFileNew()
{
	if (m_pDocManager != NULL)
		m_pDocManager->OnFileNew();
}

void CWinApp::OnFileOpen()
{
	std::string newName = "C:\\Users\\Public\\Desktop\\SaveFile";
	// 获取文件名
	//if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
	//	OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
	//	return; // open cancelled
	AfxGetApp()->OpenDocumentFile(newName.c_str());
}
