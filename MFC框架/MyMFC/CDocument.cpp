#include "CDocument.h"
#include "CWinApp.h"

IMPLEMENT_DYNCREATE(CDocument, CCmdTarget)

CDocument::CDocument()
{
	
}

CDocument::~CDocument()
{
	
}

void CDocument::OnFileSave()
{
	DoFileSave();
}

BOOL CDocument::DoFileSave()
{
	if (!DoSave(NULL)) {
		return FALSE;
	}
	return TRUE;
}


BOOL CDocument::DoSave(LPCTSTR lpszPathName, BOOL bReplace) {
	// 获取文件名
	//if (!AfxGetApp()->DoPromptFileName(newName,
	//	bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
	//	OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
	//	return FALSE;       // don't even attempt to save

	return OnSaveDocument("C:\\Users\\Public\\Desktop\\SaveFile");
}

BOOL CDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
	std::fstream* pFile = GetFile(lpszPathName, std::ios::out | std::ios::trunc);

	CArchive saveArchive(pFile, CArchive::store);
	Serialize(saveArchive);
	saveArchive.Close();
	ReleaseFile(pFile);
	return TRUE;
}

BOOL CDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	std::fstream* pFile = GetFile(lpszPathName, std::ios::in );
	CArchive loadArchive(pFile, CArchive::load);
	Serialize(loadArchive);
	loadArchive.Close();
	ReleaseFile(pFile);
	return TRUE;
}

std::fstream* CDocument::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	std::fstream* pFile = new std::fstream;
	pFile->open(lpszFileName, nOpenFlags);
	bool b = pFile->is_open();
	
	if (pFile->is_open())
		return pFile;
	return nullptr;
}

void CDocument::ReleaseFile(std::fstream* pFile)
{
	delete pFile;
}
