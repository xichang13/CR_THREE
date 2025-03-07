#pragma once
#include "CCmdTarget.h"
#include "CDocTemplate.h"
#include <fstream>

class CDocument :
    public CCmdTarget
{
	DECLARE_DYNCREATE(CDocument)
public:
	CDocument();
	~CDocument();
	void OnFileSave(); 
	virtual BOOL DoFileSave();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual std::fstream* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags);
	virtual void ReleaseFile(std::fstream* pFile);
public:
	CDocTemplate* m_pDocTemplate;
};

