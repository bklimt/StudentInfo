
#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <afxwin.h>
#include "IndexedListBox.h"

class CLoginDlg : public CDialog {
	public:
		CLoginDlg();
		BOOL OnInitDialog();
		void OnShowWindow( BOOL bShow, UINT nStatus );
		void OnOK();
		void OnCancel();
		void OnOptionsClicked();
	private:
		DECLARE_MESSAGE_MAP()
};

#endif
