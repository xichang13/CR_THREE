#include "CObject.h"

std::list<CRuntimeClass*>* g_classList;

const struct CRuntimeClass CObject::classCObject =
{ "CObject", sizeof(CObject), 0xffff, NULL, NULL, NULL };

CRuntimeClass* CObject::GetRuntimeClass() const
{
	return RUNTIME_CLASS(CObject);
}

CObject::CObject()
{
	
}

CObject::~CObject()
{
	
}

BOOL CObject::IsKindOf(const CRuntimeClass* pClass) const
{
	return 0;
}

void CArchive::Write(const void* lpBuf, UINT nMax)
{
	m_pFile->write((const char*)lpBuf, nMax);
}

void CArchive::WriteCount(DWORD_PTR dwCount)
{
	m_pFile->write((const char*)&dwCount, sizeof(dwCount));
}

void CArchive::WriteObject(const CObject* pOb)
{
	CRuntimeClass* pClassRef = pOb->GetRuntimeClass();
	WriteClass(pClassRef);
	((CObject*)pOb)->Serialize(*this);
}

void CArchive::WriteClass(const CRuntimeClass* pClassRef)
{
	m_pFile->write("FFFF", strlen("FFFF"));
	pClassRef->Store(*this);
}

void CArchive::Read(void* lpBuf, UINT nMax)
{
	m_pFile->read((char*)lpBuf, nMax);
}

DWORD_PTR CArchive::ReadCount()
{
	DWORD_PTR dwCount;
	m_pFile->read((char*) & dwCount, sizeof(dwCount));
	return dwCount;
}

CObject* CArchive::ReadObject(const CRuntimeClass* pClass)
{
	CRuntimeClass* pClassRef = ReadClass(pClass);
	if (pClassRef == NULL)
		return nullptr;
	CObject* pOb = pClassRef->CreateObject();
	pOb->Serialize(*this);
	return pOb;
}

CRuntimeClass* CArchive::ReadClass(const CRuntimeClass* pClassRefRequested, UINT* pSchema, DWORD* pObTag)
{
	CRuntimeClass* pClassRef;
	DWORD dwTag;
	m_pFile->read((char*) & dwTag, sizeof(dwTag));
	pClassRef = CRuntimeClass::Load(*this);
	return pClassRef;
}

BOOL CArchive::IsLoading() const
{
	return (m_nMode & CArchive::load) != 0;
}

BOOL CArchive::IsStoring() const
{
	return (m_nMode & CArchive::load) == 0;
}

void CArchive::Close()
{
	m_pFile->close();
}

CArchive::CArchive(std::fstream* pFile, UINT nMode, int nBufSize, void* lpBuf)
{
	if (pFile == nullptr)
		printf("file is not open");

	m_strFileName = "C:\\Users\\Public\\Desktop\\SaveFile";
	m_nMode = nMode;
	m_pFile = pFile;
}
