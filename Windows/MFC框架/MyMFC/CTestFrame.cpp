#include "CTestFrame.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CTestFrame, CFrameWnd)

const AFX_MSGMAP_ENTRY CTestFrame::_messageEntries[] = {
	{WM_CREATE, 0, 0, 0, AfxSig_bpv, (AFX_PMSG)((void(CTestFrame::*)()) (&CTestFrame::OnCreate))},
	{WM_DESTROY, 0, 0, 0, AfxSig_bv, (AFX_PMSG)((void(CTestFrame::*)()) (&CTestFrame::OnDestroy))},
	{WM_COMMAND, BN_CLICKED, 100, 100, AfxSig_vv, (AFX_PMSG)((void(CTestFrame::*)()) (&CTestFrame::OnButton1))},
	{WM_COMMAND, 0, 40003, 40003, AfxSig_vv, (AFX_PMSG)((void(CTestFrame::*)()) (&CTestFrame::OnSave))},
	{WM_COMMAND, 0, 40004, 40004, AfxSig_vv, (AFX_PMSG)((void(CTestFrame::*)()) (&CTestFrame::OnOpen))},
	{ 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
};

const AFX_MSGMAP CTestFrame::messageMap = { NULL, _messageEntries };

const AFX_MSGMAP* CTestFrame::GetMessageMap() const
{
	return &messageMap;
}

BOOL CTestFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}
