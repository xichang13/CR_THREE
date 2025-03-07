#pragma once
#include "CFrameWnd.h"
#include "CView.h"
#include "CWinApp.h"
#include "CSingleDocTemplate.h"

class CTestFrame :
    public CFrameWnd
{
	DECLARE_DYNCREATE(CTestFrame)
public:
	const AFX_MSGMAP* GetMessageMap() const;
	static const AFX_MSGMAP messageMap;
	static const AFX_MSGMAP_ENTRY _messageEntries[];

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT OnCreate(LPCREATESTRUCT lpCreate) {
		CCreateContext* pContext = (CCreateContext * )lpCreate->lpCreateParams;
		CView* pView = (CView*)(pContext->m_pNewViewClass->CreateObject());
		pView->CreateEx(0, "BUTTON", "button1", WS_CHILD | WS_VISIBLE,
			0, 0, 200, 100,
			GetSafeHwnd(),
			(HMENU)100, NULL);
		return FALSE;
	}
	virtual LRESULT OnDestroy() {
		::MessageBox(NULL, "OnDestroy", "NULL", MB_OK);
		return FALSE;
	}
	void OnButton1() {
		::MessageBox(NULL, "OnButton1", NULL, MB_OK);
	}
	void OnSave() {
		CSingleDocTemplate* pDoc = (CSingleDocTemplate*)AfxGetApp()->m_pDocManager->m_templateList.front();
		pDoc->m_pOnlyDoc->OnFileSave();
	}
	void OnOpen() {
		AfxGetApp()->OnFileOpen();
	}
};

