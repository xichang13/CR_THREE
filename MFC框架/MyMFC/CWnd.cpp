#include "CWnd.h"
#include "CSingleDocTemplate.h"
#include "CWinApp.h"
#include "resource.h"
#include <unordered_map>

thread_local std::unordered_map<HWND, CWnd*> g_WndMap;
thread_local HHOOK m_hHookOldCbtFilter;
thread_local CWnd* m_pWndInit = NULL;

IMPLEMENT_DYNCREATE(CWnd, CCmdTarget)

CWnd::CWnd()
{
	
}

CWnd::~CWnd()
{
	
}

LRESULT CALLBACK CWnd::afxWndProc(
	HWND hwnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	LRESULT Result = FALSE;
	CWnd* pWnd = g_WndMap[hwnd];
	if (pWnd == NULL) {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	const AFX_MSGMAP_ENTRY* pFindEntry = NULL;
	const AFX_MSGMAP*pMsgMap = pWnd->GetMessageMap();
	while (pMsgMap != NULL) {
		const AFX_MSGMAP_ENTRY* pEntry = pMsgMap->lpEntries;
		while (pEntry->nSig != AfxSig_end) {
			if (uMsg != WM_COMMAND) {
				if (pEntry->nMessage == uMsg) {
					pFindEntry = pEntry;
					goto BREAK;
				}
			}
			else {
				WORD wNotifyCode = HIWORD(wParam);
				WORD wID = LOWORD(wParam);
				HWND hwndCtl = (HWND)lParam;
				
				if (pEntry->nMessage == uMsg &&
					wNotifyCode == pEntry->nCode &&
					(wID >= pEntry->nID && wID <= pEntry->nLastID)) {
					pFindEntry = pEntry;
					goto BREAK;
				}
			}
			pEntry++;
		}
		pMsgMap = pMsgMap->pBase;
	}

BREAK:

	if (pFindEntry != NULL) {
		switch (pFindEntry->nSig)
		{
			case AfxSig_bpv: {
				LRESULT(CCmdTarget::*pfn)(LPCREATESTRUCT) = (LRESULT(CCmdTarget::*)(LPCREATESTRUCT))pFindEntry->pfn;
				Result = (pWnd->*pfn)((LPCREATESTRUCT)lParam);
				break;
			}
			case AfxSig_bv: {
				LRESULT(CCmdTarget:: * pfn)() = (LRESULT(CCmdTarget::*)())pFindEntry->pfn;
				Result = (pWnd->*pfn)();
				break;
			}
			case AfxSig_vv: {
				void(CCmdTarget:: * pfn)() = (void(CCmdTarget::*)())pFindEntry->pfn;
				(pWnd->*pfn)();
				return TRUE;
			}
		}
	}
	
	//WORD wLow = LOWORD(wParam);
	//switch (uMsg)
	//{
	//case WM_COMMAND: {

	//	WORD wNotifyCode = HIWORD(wParam);
	//	WORD wID = LOWORD(wParam);
	//	HWND hwndCtl = (HWND)lParam;
	//	switch (wLow) {
	//	case ID_FILE_OPEN:
	//		AfxGetApp()->OnFileOpen();
	//		break;
	//	case ID_FILE_SAVE:
	//		CSingleDocTemplate* pDoc = (CSingleDocTemplate*)AfxGetApp()->m_pDocManager->m_templateList.front();
	//		pDoc->m_pOnlyDoc->OnFileSave();
	//		break;
	//	}
	//	break;
	//}
	//default:
	//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	//}
	if (Result)
		return Result;

	if (pWnd->m_pfnSuper != NULL)
		return pWnd->m_pfnSuper(hwnd, uMsg, wParam, lParam);

	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}



BOOL CWnd::ShowWindow(int nCmdShow)
{
	return ::ShowWindow(GetSafeHwnd(), nCmdShow);
}

void CWnd::UpdateWindow() 
{
	::UpdateWindow(GetSafeHwnd());
}

BOOL CWnd::DestroyWindow()
{
	return ::DestroyWindow(GetSafeHwnd());
}

HWND CWnd::GetSafeHwnd() const
{
	return this == NULL ? NULL : m_hWnd;
}


LRESULT CALLBACK
_AfxCbtFilterHook(int code, WPARAM wParam, LPARAM lParam) {
	if (code != HCBT_CREATEWND) {
		return CallNextHookEx(m_hHookOldCbtFilter, code,
			wParam, lParam);
	}

	LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;
	HWND hWnd = (HWND)wParam;

	char szClassName[MAXCHAR];
	::GetClassName(hWnd, szClassName, sizeof(szClassName));

	if (m_pWndInit != NULL) {
		if (GetClassLong((HWND)wParam, GCL_STYLE) & CS_IME)
			goto lCallNextHook;
		g_WndMap[hWnd] = m_pWndInit;
		m_pWndInit->m_hWnd = hWnd;

		WNDPROC* pOldWndProc = &m_pWndInit->m_pfnSuper;
		WNDPROC oldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC,
			(DWORD_PTR)CWnd::afxWndProc);

		if (oldWndProc != CWnd::afxWndProc)
			*pOldWndProc = oldWndProc;

		m_pWndInit = NULL;
	}

lCallNextHook:

	return CallNextHookEx(m_hHookOldCbtFilter, code,
		wParam, lParam);
}

void AfxHookWindowCreate(CWnd* pWnd) {
	m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT,
		_AfxCbtFilterHook, NULL, ::GetCurrentThreadId());
	m_pWndInit = pWnd;
}

BOOL CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam)
{
	CREATESTRUCT cs;
	cs.dwExStyle = dwExStyle;
	cs.lpszClass = lpszClassName;
	cs.lpszName = lpszWindowName;
	cs.style = dwStyle;
	cs.x = x;
	cs.y = y;
	cs.cx = nWidth;
	cs.cy = nHeight;
	cs.hwndParent = hWndParent;
	cs.hMenu = nIDorHMenu;
	cs.hInstance = AfxGetInstanceHandle();
	cs.lpCreateParams = lpParam;

	if (!PreCreateWindow(cs))
	{
		return FALSE;
	}

	AfxHookWindowCreate(this);
	HWND hWnd = CreateWindowEx(cs.dwExStyle, cs.lpszClass,
		cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
		cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

	::UnhookWindowsHookEx(m_hHookOldCbtFilter);
	m_hHookOldCbtFilter = NULL;
	return TRUE;
}

BOOL CWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if ((cs.hMenu == NULL) && (cs.style & WS_CHILD))
	{
		cs.hMenu = (HMENU)(UINT_PTR)this;
	}

	return TRUE;
}
