
#include <afxwin.h>
#include "User.h"

class CUserEditDlg : public CDialog {
	public:
		CUserEditDlg();
		BOOL OnInitDialog();
		void OnOK();
		void DoModal( string username );
	private:
		CUser editUser;
};
