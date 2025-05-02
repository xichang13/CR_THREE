#pragma once
#include "CObject.h"

class CCmdTarget;

typedef void (CCmdTarget::*AFX_PMSG)(void);

struct AFX_MSGMAP_ENTRY;       // declared below after CWnd

struct AFX_MSGMAP
{
	const AFX_MSGMAP* pBase;
	const AFX_MSGMAP_ENTRY* lpEntries;
};

struct AFX_MSGMAP_ENTRY
{
	UINT nMessage;   // windows message
	UINT nCode;      // control code or WM_NOTIFY code
	UINT nID;        // control ID (or 0 for windows messages)
	UINT nLastID;    // used for entries specifying a range of control id's
	UINT_PTR nSig;       // signature type (action) or pointer to message #
	AFX_PMSG pfn;    // routine to call (or special value)
};

class CCmdTarget :
    public CObject
{
	DECLARE_DYNCREATE(CCmdTarget)
public:
	CCmdTarget();
	~CCmdTarget();
	virtual const AFX_MSGMAP* GetMessageMap() const { return NULL; }
	virtual LRESULT OnCreate(LPCREATESTRUCT lpCreate) { return FALSE; }
	virtual LRESULT OnDestroy() { return FALSE; }
	virtual LRESULT OnClose() { return FALSE; }
	virtual LRESULT OnCommand(DWORD wNotifyCode, DWORD wID, HWND hwndCtl) { return FALSE; }
};

enum AfxSig
{
	AfxSig_end = 0,     // [marks end of message map]

	AfxSig_b_D_v,				// BOOL (CDC*)
	AfxSig_b_b_v,				// BOOL (BOOL)
	AfxSig_b_u_v,				// BOOL (UINT)
	AfxSig_b_h_v,				// BOOL (HANDLE)
	AfxSig_b_W_uu,				// BOOL (CWnd*, UINT, UINT)
	AfxSig_b_W_COPYDATASTRUCT,				// BOOL (CWnd*, COPYDATASTRUCT*)
	AfxSig_b_v_HELPINFO,		// BOOL (LPHELPINFO);
	AfxSig_CTLCOLOR,			// HBRUSH (CDC*, CWnd*, UINT)
	AfxSig_CTLCOLOR_REFLECT,	// HBRUSH (CDC*, UINT)
	AfxSig_i_u_W_u,				// int (UINT, CWnd*, UINT)  // ?TOITEM
	AfxSig_i_uu_v,				// int (UINT, UINT)
	AfxSig_i_W_uu,				// int (CWnd*, UINT, UINT)
	AfxSig_i_v_s,				// int (LPTSTR)
	AfxSig_l_w_l,				// LRESULT (WPARAM, LPARAM)
	AfxSig_l_uu_M,				// LRESULT (UINT, UINT, CMenu*)
	AfxSig_v_b_h,				// void (BOOL, HANDLE)
	AfxSig_v_h_v,				// void (HANDLE)
	AfxSig_v_h_h,				// void (HANDLE, HANDLE)
	AfxSig_v_v_v,				// void ()
	AfxSig_v_u_v,				// void (UINT)
	AfxSig_v_up_v,				// void (UINT_PTR)
	AfxSig_v_u_u,				// void (UINT, UINT)
	AfxSig_v_uu_v,				// void (UINT, UINT)
	AfxSig_v_v_ii,				// void (int, int)
	AfxSig_v_u_uu,				// void (UINT, UINT, UINT)
	AfxSig_v_u_ii,				// void (UINT, int, int)
	AfxSig_v_u_W,				// void (UINT, CWnd*)
	AfxSig_i_u_v,				// int (UINT)
	AfxSig_u_u_v,				// UINT (UINT)
	AfxSig_b_v_v,				// BOOL ()
	AfxSig_v_w_l,				// void (WPARAM, LPARAM)
	AfxSig_MDIACTIVATE,			// void (BOOL, CWnd*, CWnd*)
	AfxSig_v_D_v,				// void (CDC*)
	AfxSig_v_M_v,				// void (CMenu*)
	AfxSig_v_M_ub,				// void (CMenu*, UINT, BOOL)
	AfxSig_v_W_v,				// void (CWnd*)
	AfxSig_v_v_W,				// void (CWnd*)
	AfxSig_v_W_uu,				// void (CWnd*, UINT, UINT)
	AfxSig_v_W_p,				// void (CWnd*, CPoint)
	AfxSig_v_W_h,				// void (CWnd*, HANDLE)
	AfxSig_C_v_v,				// HCURSOR ()
	AfxSig_ACTIVATE,			// void (UINT, CWnd*, BOOL)
	AfxSig_SCROLL,				// void (UINT, UINT, CWnd*)
	AfxSig_SCROLL_REFLECT,		// void (UINT, UINT)
	AfxSig_v_v_s,				// void (LPTSTR)
	AfxSig_v_u_cs,				// void (UINT, LPCTSTR)
	AfxSig_OWNERDRAW,			// void (int, LPTSTR) force return TRUE
	AfxSig_i_i_s,				// int (int, LPTSTR)
	AfxSig_u_v_p,				// UINT (CPoint)
	AfxSig_u_v_v,				// UINT ()
	AfxSig_v_b_NCCALCSIZEPARAMS,		// void (BOOL, NCCALCSIZE_PARAMS*)
	AfxSig_v_v_WINDOWPOS,			// void (WINDOWPOS*)
	AfxSig_v_uu_M,				// void (UINT, UINT, HMENU)
	AfxSig_v_u_p,				// void (UINT, CPoint)
	AfxSig_SIZING,				// void (UINT, LPRECT)
	AfxSig_MOUSEWHEEL,			// BOOL (UINT, short, CPoint)
	AfxSig_MOUSEHWHEEL,			// void (UINT, short, CPoint)
	AfxSigCmd_v,				// void ()
	AfxSigCmd_b,				// BOOL ()
	AfxSigCmd_RANGE,			// void (UINT)
	AfxSigCmd_EX,				// BOOL (UINT)
	AfxSigNotify_v,				// void (NMHDR*, LRESULT*)
	AfxSigNotify_b,				// BOOL (NMHDR*, LRESULT*)
	AfxSigNotify_RANGE,			// void (UINT, NMHDR*, LRESULT*)
	AfxSigNotify_EX,			// BOOL (UINT, NMHDR*, LRESULT*)
	AfxSigCmdUI,				// void (CCmdUI*)
	AfxSigCmdUI_RANGE,			// void (CCmdUI*, UINT)
	AfxSigCmd_v_pv,				// void (void*)
	AfxSigCmd_b_pv,				// BOOL (void*)
	AfxSig_l,				// LRESULT ()
	AfxSig_l_p,				// LRESULT (CPOINT)	
	AfxSig_u_W_u,				// UINT (CWnd*, UINT)
	AfxSig_v_u_M,				// void (UINT, CMenu* )
	AfxSig_u_u_M,				// UINT (UINT, CMenu* )
	AfxSig_u_v_MENUGETOBJECTINFO,		// UINT (MENUGETOBJECTINFO*)
	AfxSig_v_M_u,				// void (CMenu*, UINT)
	AfxSig_v_u_LPMDINEXTMENU,		// void (UINT, LPMDINEXTMENU)
	AfxSig_APPCOMMAND,			// void (CWnd*, UINT, UINT, UINT)
	AfxSig_RAWINPUT,			// void (UINT, HRAWINPUT)
	AfxSig_u_u_u,				// UINT (UINT, UINT)
	AfxSig_MOUSE_XBUTTON,			// void (UINT, UINT, CPoint)
	AfxSig_MOUSE_NCXBUTTON,			// void (short, UINT, CPoint)
	AfxSig_INPUTLANGCHANGE,			// void (UINT, UINT)
	AfxSig_v_u_hkl,					// void (UINT, HKL)
	AfxSig_INPUTDEVICECHANGE,		// void (unsigned short, HANDLE)
	AfxSig_l_D_u,					// LRESULT (CDC*, UINT)
	AfxSig_i_v_S,				// int (LPCTSTR)
	AfxSig_v_F_b,				// void (CFont*, BOOL)
	AfxSig_h_v_v,				// HANDLE ()
	AfxSig_h_b_h,				// HANDLE (BOOL, HANDLE)
	AfxSig_b_v_ii,				// BOOL (int, int)
	AfxSig_h_h_h,				// HANDLE (HANDLE, HANDLE)
	AfxSig_MDINext,				// void (CWnd*, BOOL)
	AfxSig_u_u_l,				// UINT (UINT, LPARAM)

	// Old
	AfxSig_bD = AfxSig_b_D_v,      // BOOL (CDC*)
	AfxSig_bb = AfxSig_b_b_v,      // BOOL (BOOL)
	AfxSig_bWww = AfxSig_b_W_uu,    // BOOL (CWnd*, UINT, UINT)
	AfxSig_hDWw = AfxSig_CTLCOLOR,    // HBRUSH (CDC*, CWnd*, UINT)
	AfxSig_hDw = AfxSig_CTLCOLOR_REFLECT,     // HBRUSH (CDC*, UINT)
	AfxSig_iwWw = AfxSig_i_u_W_u,    // int (UINT, CWnd*, UINT)
	AfxSig_iww = AfxSig_i_uu_v,     // int (UINT, UINT)
	AfxSig_iWww = AfxSig_i_W_uu,    // int (CWnd*, UINT, UINT)
	AfxSig_is = AfxSig_i_v_s,      // int (LPTSTR)
	AfxSig_lwl = AfxSig_l_w_l,     // LRESULT (WPARAM, LPARAM)
	AfxSig_lwwM = AfxSig_l_uu_M,    // LRESULT (UINT, UINT, CMenu*)
	AfxSig_vv = AfxSig_v_v_v,      // void (void)

	AfxSig_vw = AfxSig_v_u_v,      // void (UINT)
	AfxSig_vww = AfxSig_v_u_u,     // void (UINT, UINT)
	AfxSig_vww2 = AfxSig_v_uu_v,    // void (UINT, UINT) // both come from wParam
	AfxSig_vvii = AfxSig_v_v_ii,    // void (int, int) // wParam is ignored
	AfxSig_vwww = AfxSig_v_u_uu,    // void (UINT, UINT, UINT)
	AfxSig_vwii = AfxSig_v_u_ii,    // void (UINT, int, int)
	AfxSig_vwl = AfxSig_v_w_l,     // void (UINT, LPARAM)
	AfxSig_vbWW = AfxSig_MDIACTIVATE,    // void (BOOL, CWnd*, CWnd*)
	AfxSig_vD = AfxSig_v_D_v,      // void (CDC*)
	AfxSig_vM = AfxSig_v_M_v,      // void (CMenu*)
	AfxSig_vMwb = AfxSig_v_M_ub,    // void (CMenu*, UINT, BOOL)

	AfxSig_vW = AfxSig_v_W_v,      // void (CWnd*)
	AfxSig_vWww = AfxSig_v_W_uu,    // void (CWnd*, UINT, UINT)
	AfxSig_vWp = AfxSig_v_W_p,     // void (CWnd*, CPoint)
	AfxSig_vWh = AfxSig_v_W_h,     // void (CWnd*, HANDLE)
	AfxSig_vwW = AfxSig_v_u_W,     // void (UINT, CWnd*)
	AfxSig_vwWb = AfxSig_ACTIVATE,    // void (UINT, CWnd*, BOOL)
	AfxSig_vwwW = AfxSig_SCROLL,    // void (UINT, UINT, CWnd*)
	AfxSig_vwwx = AfxSig_SCROLL_REFLECT,    // void (UINT, UINT)
	AfxSig_vs = AfxSig_v_v_s,      // void (LPTSTR)
	AfxSig_vOWNER = AfxSig_OWNERDRAW,  // void (int, LPTSTR), force return TRUE
	AfxSig_iis = AfxSig_i_i_s,     // int (int, LPTSTR)
	AfxSig_wp = AfxSig_u_v_p,      // UINT (CPoint)
	AfxSig_wv = AfxSig_u_v_v,      // UINT (void)
	AfxSig_vPOS = AfxSig_v_v_WINDOWPOS,    // void (WINDOWPOS*)
	AfxSig_vCALC = AfxSig_v_b_NCCALCSIZEPARAMS,   // void (BOOL, NCCALCSIZE_PARAMS*)
	AfxSig_vNMHDRpl = AfxSigNotify_v,    // void (NMHDR*, LRESULT*)
	AfxSig_bNMHDRpl = AfxSigNotify_b,    // BOOL (NMHDR*, LRESULT*)
	AfxSig_vwNMHDRpl = AfxSigNotify_RANGE,   // void (UINT, NMHDR*, LRESULT*)
	AfxSig_bwNMHDRpl = AfxSigNotify_EX,   // BOOL (UINT, NMHDR*, LRESULT*)
	AfxSig_bHELPINFO = AfxSig_b_v_HELPINFO,   // BOOL (HELPINFO*)
	AfxSig_vwSIZING = AfxSig_SIZING,    // void (UINT, LPRECT) -- return TRUE

	// signatures specific to CCmdTarget
	AfxSig_cmdui = AfxSigCmdUI,   // void (CCmdUI*)
	AfxSig_cmduiw = AfxSigCmdUI_RANGE,  // void (CCmdUI*, UINT)
	AfxSig_vpv = AfxSigCmd_v_pv,     // void (void*)
	AfxSig_bpv = AfxSigCmd_b_pv,     // BOOL (void*)

	// Other aliases (based on implementation)
	AfxSig_vwwh = AfxSig_v_uu_M,                // void (UINT, UINT, HMENU)
	AfxSig_vwp = AfxSig_v_u_p,                 // void (UINT, CPoint)
	AfxSig_bw = AfxSig_b_u_v,      // BOOL (UINT)
	AfxSig_bh = AfxSig_b_h_v,      // BOOL (HANDLE)
	AfxSig_iw = AfxSig_i_u_v,      // int (UINT)
	AfxSig_ww = AfxSig_u_u_v,      // UINT (UINT)
	AfxSig_bv = AfxSig_b_v_v,      // BOOL (void)
	AfxSig_hv = AfxSig_C_v_v,      // HANDLE (void)
	AfxSig_vb = AfxSig_vw,      // void (BOOL)
	AfxSig_vbh = AfxSig_v_b_h,    // void (BOOL, HANDLE)
	AfxSig_vbw = AfxSig_vww,    // void (BOOL, UINT)
	AfxSig_vhh = AfxSig_v_h_h,    // void (HANDLE, HANDLE)
	AfxSig_vh = AfxSig_v_h_v,      // void (HANDLE)
	AfxSig_viSS = AfxSig_vwl,   // void (int, STYLESTRUCT*)
	AfxSig_bwl = AfxSig_lwl,
	AfxSig_vwMOVING = AfxSig_vwSIZING,  // void (UINT, LPRECT) -- return TRUE

	AfxSig_vW2 = AfxSig_v_v_W,                 // void (CWnd*) (CWnd* comes from lParam)
	AfxSig_bWCDS = AfxSig_b_W_COPYDATASTRUCT,               // BOOL (CWnd*, COPYDATASTRUCT*)
	AfxSig_bwsp = AfxSig_MOUSEWHEEL,                // BOOL (UINT, short, CPoint)
	AfxSig_vws = AfxSig_v_u_cs,
};

