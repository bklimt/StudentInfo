
#pragma warning(disable:4786)

#include <afxwin.h>
#include <vector>
#include <string>
using namespace std;

class CUserListDlg : public CDialog {
	public:
		CUserListDlg();
		BOOL OnInitDialog();
		void OnSize( UINT nType, int cx, int cy );
		void OnUserListDblClk();
		void OnNewUserButton();
		void OnDeleteUserButton();
		void RefreshUserList();
	private:
		DECLARE_MESSAGE_MAP()
};
