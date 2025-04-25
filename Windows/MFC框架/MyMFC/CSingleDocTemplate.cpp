#include "CSingleDocTemplate.h"

IMPLEMENT_DYNCREATE(CSingleDocTemplate, CDocTemplate)

CSingleDocTemplate::CSingleDocTemplate()
{
}

CSingleDocTemplate::~CSingleDocTemplate()
{
}

CSingleDocTemplate::CSingleDocTemplate(UINT nIDResource,
	CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass,
	CRuntimeClass* pViewClass)
	: CDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
	m_pOnlyDoc = NULL;
}

void CSingleDocTemplate::AddDocument(CDocument* pDoc)
{
	CDocTemplate::AddDocument(pDoc);
	m_pOnlyDoc = pDoc;
}

