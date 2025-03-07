#include "CDocTemplate.h"
#include "CWinThread.h"

IMPLEMENT_DYNCREATE(CDocTemplate, CCmdTarget)

CDocTemplate::CDocTemplate()
{
}

CDocTemplate::~CDocTemplate()
{
}

CDocTemplate::CDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) 
{
	m_nIDResource = nIDResource;
	m_pDocClass = pDocClass;
	m_pFrameClass = pFrameClass;
	m_pViewClass = pViewClass;
}

CDocument* CDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	return OpenDocumentFile(lpszPathName, TRUE, bMakeVisible);
}

CDocument* CDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible)
{
	CDocument* pDocument = CreateNewDocument();
	CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);		
	if (pFrame == NULL)
	{
		delete pDocument;
		return NULL;
	}
	CWinThread* pThread = AfxGetThread();
	pThread->m_pMainWnd = pFrame;
	return pDocument;
}

CDocument* CDocTemplate::CreateNewDocument()
{
	CDocument* pDocument = (CDocument*)m_pDocClass->CreateObject();
	AddDocument(pDocument);
	return pDocument;
}

void CDocTemplate::AddDocument(CDocument* pDoc)
{
	pDoc->m_pDocTemplate = this;
}

CFrameWnd* CDocTemplate::CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther)
{
	CCreateContext context;
	context.m_pCurrentFrame = pOther;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = m_pViewClass;
	context.m_pNewDocTemplate = this;

	CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
	if (!pFrame->LoadFrame(m_nIDResource,
		WS_OVERLAPPEDWINDOW,   // default frame styles
		NULL, &context))
	{
		printf("Warning: CDocTemplate couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	return pFrame;
}
