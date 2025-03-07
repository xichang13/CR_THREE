#pragma once
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <list>

class CObject;
class CWnd;
class CWinApp;
class CCmdTarget;
class CView;
class CWinThread;
class CDocument;
class CDocTemplate;
class CFrameWnd;
struct CRuntimeClass;
class CArchive;

CWinApp* AfxGetApp();
HINSTANCE AfxGetInstanceHandle();
CWinThread* AfxGetThread();

extern std::list<CRuntimeClass*>* g_classList;

struct AFX_CLASSINIT
{
	AFX_CLASSINIT(CRuntimeClass* pNewClass) {
		if (g_classList == NULL) {
			g_classList = new std::list<CRuntimeClass*>();
		}
		g_classList->push_front(pNewClass);
	}
};

class CArchive {
public:
	enum Mode { store = 0, load = 1 };
	CArchive(std::fstream* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL);

	void Write(const void* lpBuf, UINT nMax);
	void WriteCount(DWORD_PTR dwCount);
	void WriteObject(const CObject* pOb);
	void WriteClass(const CRuntimeClass* pClassRef);

	void Read(void* lpBuf, UINT nMax);
	DWORD_PTR ReadCount();
	CObject* ReadObject(const CRuntimeClass* pClass);
	CRuntimeClass* ReadClass(const CRuntimeClass* pClassRefRequested = NULL,
		UINT* pSchema = NULL, DWORD* pObTag = NULL);

	BOOL IsLoading() const;
	BOOL IsStoring() const;
	void Close();

	BOOL m_nMode;
	std::fstream* m_pFile;
	std::string m_strFileName;
};

struct CRuntimeClass
{
	LPCSTR m_lpszClassName;
	int m_nObjectSize;
	UINT m_wSchema; 
	CObject* (PASCAL* m_pfnCreateObject)(); 
	CRuntimeClass* m_pBaseClass;
	CObject* CreateObject() {
		return (*m_pfnCreateObject)();
	}
	static CRuntimeClass* PASCAL FromName(LPCSTR lpszClassName) {
		for (CRuntimeClass* pClass : *g_classList) {
			if (strcmp(pClass->m_lpszClassName, lpszClassName) == 0) {
				return pClass;
			}
		}
		return NULL;
	}
	void Store(CArchive& ar) const {
		WORD nLen = (WORD)::strlen(m_lpszClassName);
		ar.Write(&nLen, sizeof(nLen));
		ar.Write(m_lpszClassName, nLen);
	}
	static CRuntimeClass* PASCAL Load(CArchive& ar) {
		WORD nLen;
		char szClassName[64];
		ar.Read(&nLen, sizeof(nLen));
		ar.Read(szClassName, nLen);
		szClassName[nLen] = '\0';

		CRuntimeClass* pClass = FromName(szClassName);
		if (pClass == NULL)
		{
			// not found, trace a warning for diagnostic purposes
			printf("Warning: Cannot load %hs from archive.  Class not defined.\n",
				szClassName);
		}
		return pClass;
	}
	CRuntimeClass* m_pNextClass;
};

struct CCreateContext   // Creation information structure
	// All fields are optional and may be NULL
{
	// for creating new views
	CRuntimeClass* m_pNewViewClass; // runtime class of view to create or NULL
	CDocument* m_pCurrentDoc;

	// for creating MDI children (CMDIChildWnd::LoadFrame)
	CDocTemplate* m_pNewDocTemplate;

	// for sharing view/frame state from the original view/frame
	CView* m_pLastView;
	CFrameWnd* m_pCurrentFrame;

	// Implementation
	CCreateContext() { memset(this, 0, sizeof(*this)); }
};


class CObject
{
public:
	CObject();
	~CObject();
	BOOL IsKindOf(const CRuntimeClass* pClass) const;
public:
	static const CRuntimeClass classCObject;
	virtual CRuntimeClass* GetRuntimeClass() const;
	virtual void Serialize(CArchive& ar) {}
};

#define RUNTIME_CLASS(class_name) ((CRuntimeClass*)(&class_name::class##class_name))

#define DECLARE_DYNAMIC(class_name) \
public: \
	static const CRuntimeClass class##class_name; \
	virtual CRuntimeClass* GetRuntimeClass() const; \

#define DECLARE_DYNCREATE(class_name) \
	DECLARE_DYNAMIC(class_name) \
	static CObject* PASCAL CreateObject();

#define IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
	const CRuntimeClass class_name::class##class_name = { \
		#class_name, sizeof(class class_name), wSchema, pfnNew, \
			RUNTIME_CLASS(base_class_name), NULL }; \
	CRuntimeClass* class_name::GetRuntimeClass() const \
		{ return RUNTIME_CLASS(class_name); }

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
	CObject* class_name::CreateObject() \
		{ return new class_name; } \
	IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
		class_name::CreateObject)

#define DECLARE_SERIAL(class_name) \
	DECLARE_DYNCREATE(class_name) 

#define IMPLEMENT_SERIAL(class_name, base_class_name, wSchema) \
	CObject* PASCAL class_name::CreateObject() \
		{ return new class_name; } \
	IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, \
		class_name::CreateObject) \
	AFX_CLASSINIT _init_##class_name(RUNTIME_CLASS(class_name)); 