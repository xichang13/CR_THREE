#pragma once
#include "CCmdTarget.h"
class CWnd :
    public CCmdTarget
{
	DECLARE_DYNCREATE(CWnd)
public:
	CWnd();
	~CWnd();
	BOOL ShowWindow(int nCmdShow);
	void UpdateWindow();
	virtual BOOL DestroyWindow();
	HWND GetSafeHwnd() const;
	static LRESULT CALLBACK afxWndProc(
		HWND hwnd,        // handle to window
		UINT uMsg,        // message identifier
		WPARAM wParam,    // first message parameter
		LPARAM lParam);    // second message parameter
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		int x, int y, int nWidth, int nHeight,
		HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam = NULL);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	HWND m_hWnd;
	WNDPROC m_pfnSuper;
};

