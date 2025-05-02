#include "CFrameWnd.h"
#include "resource.h"
#include "CWinApp.h"
#include "CSingleDocTemplate.h"

IMPLEMENT_DYNCREATE(CFrameWnd, CWnd)

const RECT CFrameWnd::rectDefault = { 0, 0, 0x300, 0x300 };

CFrameWnd::CFrameWnd()
{
	
}

CFrameWnd::~CFrameWnd()
{
	
}

BOOL CFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
	CWnd* pParentWnd, CCreateContext* pContext)
{
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults
	wndcls.lpfnWndProc = &CWnd::afxWndProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszClassName = "CR49";
	wndcls.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDResource));

	if (!RegisterClass(&wndcls))
	{
		printf("RegisterClass is Not\n");
		return FALSE;
	}

	if (!Create(wndcls.lpszClassName, wndcls.lpszClassName, dwDefaultStyle, rectDefault,
		pParentWnd, MAKEINTRESOURCE(nIDResource), 0L, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	return TRUE;
}

BOOL CFrameWnd::Create(LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName,
	DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd,
	LPCTSTR lpszMenuName,
	DWORD dwExStyle,
	CCreateContext* pContext) 
{
	HMENU hMenu = NULL;
	if (lpszMenuName != NULL)
	{
		// load in a menu that will get destroyed when window gets destroyed
		HINSTANCE hInst = AfxGetInstanceHandle();
		if ((hMenu = ::LoadMenu(hInst, lpszMenuName)) == NULL)
		{
			printf("Warning: failed to load menu for CFrameWnd.\n");
			return FALSE;
		}
	}

	if (!CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(), hMenu, (LPVOID)pContext))
	{
		printf("Warning: failed to create CFrameWnd.\n");
		if (hMenu != NULL)
			DestroyMenu(hMenu);
		return FALSE;
	}
	
	return TRUE;
}
