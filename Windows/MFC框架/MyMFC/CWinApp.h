#pragma once
#include "CWinThread.h"
#include "CDocTemplate.h"
#include "CDocManager.h"
#include <string>

class CWinApp :
    public CWinThread
{
	DECLARE_DYNCREATE(CWinApp)
public:
	CWinApp();
	~CWinApp();

	void AddDocTemplate(CDocTemplate* pTemplate);
	BOOL DoPromptFileName(std::string& fileName, UINT nIDSTitle,
		DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	void OnFileNew();
	void OnFileOpen();
public:
	HINSTANCE m_hInstance;
	LPTSTR m_lpCmdLine;
	int m_nCmdShow;
	CDocManager* m_pDocManager;
};

