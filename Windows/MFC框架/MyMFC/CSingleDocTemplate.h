#pragma once
#include "CDocTemplate.h"
#include "CDocument.h"

class CSingleDocTemplate :
    public CDocTemplate
{
	DECLARE_DYNCREATE(CSingleDocTemplate)
public:
	CSingleDocTemplate();
	~CSingleDocTemplate();
	CSingleDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	void AddDocument(CDocument* pDoc);
public:
	CDocument* m_pOnlyDoc;
};

