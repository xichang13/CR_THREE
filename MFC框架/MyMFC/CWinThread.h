#pragma once
#include "CCmdTarget.h"
#include "CWnd.h"

class CWinThread :
    public CCmdTarget
{
	DECLARE_DYNCREATE(CWinThread)
public:
	CWinThread();
	~CWinThread();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
public:
	CWnd* m_pMainWnd;
};

