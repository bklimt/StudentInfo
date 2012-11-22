
#include <afxwin.h>
#include "UserListDlg.h"

class CUserListWnd : public CMDIChildWnd {
	public:
		BOOL Create( CMDIFrameWnd* parent );
		void OnClose();
		void OnPaint();
		void OnSetFocus( CWnd* pOldWnd );
		void OnSize( UINT nType, int cx, int cy );
	private:
		CUserListDlg dialog;
		DECLARE_DYNAMIC( CUserListWnd )
		DECLARE_MESSAGE_MAP()
};

#ifdef DECLARE_USERLISTWND
	CUserListWnd* userListWnd;
#else
	extern CUserListWnd* userListWnd;
#endif
