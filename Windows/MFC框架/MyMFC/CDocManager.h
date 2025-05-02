#pragma once
#include "CObject.h"
#include "CDocTemplate.h"
#include <string>

class CDocManager :
    public CObject
{
	DECLARE_DYNCREATE(CDocManager)
public:
	virtual void AddDocTemplate(CDocTemplate* pTemplate);
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual BOOL DoPromptFileName(std::string& fileName, UINT nIDSTitle,
		DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName); // open named file
public:
	std::list<CDocTemplate*> m_templateList;
};

