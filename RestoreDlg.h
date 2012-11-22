
#include <afxwin.h>
#include <afxcmn.h>
#include "RestoreHandler.h"

class CRestoreDlg : public CDialog {
	public:
		CRestoreDlg();
		void DoDataExchange( CDataExchange* pDX );
		BOOL OnInitDialog();
		void OnTimer( UINT nIDEvent );
		void OnCancel();
	private:
		int timer;
		CStatic sql;
		CProgressCtrl progress;
		CRestoreHandler handler;
		DECLARE_MESSAGE_MAP()
};