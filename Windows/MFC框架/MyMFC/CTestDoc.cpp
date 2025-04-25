#include "CTestDoc.h"
#include "CLine.h"

IMPLEMENT_SERIAL(CTestDoc, CDocument, 1)

CTestDoc::CTestDoc()
{
	
}

CTestDoc::~CTestDoc()
{
	
}

void CTestDoc::Serialize(CArchive& ar)
{
	CLine obj;
	if (ar.IsStoring())
	{
		ar.WriteCount(2);
		obj.x = 1;
		obj.y = 2;
		ar.WriteObject(&obj);
		obj.x = 3;
		obj.y = 4;
		ar.WriteObject(&obj);
	}
	else
	{
		SIZE_T nCount = ar.ReadCount();
		for (SIZE_T i = 0; i < nCount; i++) {
			CLine* pObj = (CLine*)ar.ReadObject(RUNTIME_CLASS(CLine));
		}
	}
}
