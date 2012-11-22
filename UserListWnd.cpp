
#define DECLARE_USERLISTWND

#include "UserListWnd.h"
#include "resource.h"

BOOL CUserListWnd::Create( CMDIFrameWnd* parent ) {

	BOOL retval = CMDIChildWnd::Create(
		NULL,
		"User List",
		WS_CHILD | WS_OVERLAPPEDWINDOW,
		CRect( 20, 20, 200, 200 ),
		parent,
		NULL
	);

	if ( retval ) {
		retval = dialog.Create( IDD_USERLIST, this );
	}

	CRect client;
	GetClientRect( client );
	if ( retval ) {
		dialog.SetWindowPos( &wndTop, 0, 0, client.Width(), client.Height(), SWP_SHOWWINDOW );
	}

	return retval;
}

void CUserListWnd::OnClose() {
	ShowWindow( SW_HIDE );
}

void CUserListWnd::OnPaint() {
	CMDIChildWnd::OnPaint();
	dialog.Invalidate();
}

void CUserListWnd::OnSetFocus( CWnd* pOldWnd ) {
	CMDIChildWnd::OnSetFocus( pOldWnd );
	dialog.SetFocus();
}

void CUserListWnd::OnSize( UINT nType, int cx, int cy ) {
	if ( dialog.GetSafeHwnd() ) {
		dialog.SetWindowPos( &wndTop, 0, 0, cx, cy, SWP_NOREDRAW );
	}
	CMDIChildWnd::OnSize( nType, cx, cy );
}

IMPLEMENT_DYNAMIC( CUserListWnd, CMDIChildWnd )

BEGIN_MESSAGE_MAP( CUserListWnd, CMDIChildWnd )
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()
