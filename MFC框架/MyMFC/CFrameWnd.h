#pragma once
#include "CWnd.h"
#include "CDocument.h"

class CFrameWnd :
    public CWnd
{
	DECLARE_DYNCREATE(CFrameWnd)
public:
	CFrameWnd();
	~CFrameWnd();
	virtual BOOL LoadFrame(UINT nIDResource,
		DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW,
		CWnd* pParentWnd = NULL,
		CCreateContext* pContext = NULL);

	static const RECT rectDefault;

	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW,
		const RECT& rect = rectDefault,
		CWnd* pParentWnd = NULL,        // != NULL for popups
		LPCTSTR lpszMenuName = NULL,
		DWORD dwExStyle = 0,
		CCreateContext* pContext = NULL);


protected:
	UINT m_nIDHelp;
};

