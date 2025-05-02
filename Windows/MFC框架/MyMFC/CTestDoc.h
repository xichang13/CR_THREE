#pragma once
#include "CDocument.h"
class CTestDoc :
    public CDocument
{
	DECLARE_SERIAL(CTestDoc)
public:
	CTestDoc();
	~CTestDoc();

	virtual void Serialize(CArchive& ar);
};

