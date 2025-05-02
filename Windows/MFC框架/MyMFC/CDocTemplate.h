#pragma once
#include "CCmdTarget.h"
#include "CDocument.h"
#include "CFrameWnd.h"

class CDocTemplate :
    public CCmdTarget
{
	DECLARE_DYNCREATE(CDocTemplate)
public:
	CDocTemplate();
	~CDocTemplate();
	CDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
	CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible);

	virtual CDocument* CreateNewDocument();

	virtual void AddDocument(CDocument* pDoc);

	virtual CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther);

protected:
	UINT m_nIDResource;
	CRuntimeClass* m_pDocClass;
	CRuntimeClass* m_pFrameClass;
	CRuntimeClass* m_pViewClass;
	CDocTemplate* m_pDocTemplate;
};

