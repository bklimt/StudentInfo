
#include <afxwin.h>
#include <afxcmn.h>
#include "BackupHandler.h"

class CBackupDlg : public CDialog {
	public:
		CBackupDlg();
		void DoDataExchange( CDataExchange* pDX );
		BOOL OnInitDialog();
		void OnTimer( UINT nIDEvent );
		void OnCancel();
	private:
		int timer;
		bool compress;
		string outfile;
		string tempfile;
		string zipPath;
		CBackupHandler handler;

		CListCtrl tableList;
		CProgressCtrl progress;
		DECLARE_MESSAGE_MAP()
};
