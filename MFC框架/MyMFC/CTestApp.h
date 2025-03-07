#pragma once
#include "CWinApp.h"
class CTestApp :
    public CWinApp
{
	DECLARE_DYNCREATE(CTestApp)
public:
	CTestApp();
	~CTestApp();

	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

