#include "CDocManager.h"
#include "CSingleDocTemplate.h"

IMPLEMENT_DYNCREATE(CDocManager, CObject)

void CDocManager::AddDocTemplate(CDocTemplate* pTemplate)
{
	m_templateList.push_back(pTemplate);
}

void CDocManager::OnFileNew()
{
	if (m_templateList.empty())
	{
		printf("Error: no document templates registered with CWinApp.\n");
		return;
	}

	CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.front();

	pTemplate->OpenDocumentFile(NULL);
}

void CDocManager::OnFileOpen()
{

}

BOOL CDocManager::DoPromptFileName(std::string& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	// 获取要存储的文件名
	return TRUE;
}

CDocument* CDocManager::OpenDocumentFile(LPCTSTR lpszFileName)
{
	CDocument* pDocument = ((CSingleDocTemplate*)m_templateList.front())->m_pOnlyDoc;
	pDocument->OnOpenDocument(lpszFileName);
	return NULL;
}
