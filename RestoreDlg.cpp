
#pragma warning(disable:4786)
#include <afxdlgs.h>
#include "resource.h"
#include "RestoreDlg.h"
#include "MainWindow.h"
#include "StorageController.h"
#include "Preferences.h"

extern CPreferences preferences;
extern CMainWindow* mainWnd;

CRestoreDlg::CRestoreDlg() : CDialog( IDD_RESTORE ) {
}

void CRestoreDlg::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_RESTOREPROGRESS, progress );
	DDX_Control( pDX, IDC_RESTORESQL, sql );
}

BOOL CRestoreDlg::OnInitDialog() {
	if ( !CDialog::OnInitDialog() ) {
		return FALSE;
	}

	if ( MessageBox( "You have chosen to restore a backup of the database.\nThis will delete all of the data currently in the database.\nAre you sure you want to do this?", "StudentInfo", MB_YESNO ) == IDYES ) {
		if ( MessageBox( "Are you really sure?", "StudentInfo", MB_YESNO ) == IDYES ) {
			if ( MessageBox( "Are you really really sure?", "StudentInfo", MB_YESNO ) == IDYES ) {
				string zipPath = preferences.GetHomePath() + "gzip.exe";
				bool gzipAvailable = ( GetFileAttributes( zipPath.c_str() ) != -1 );

				CFileDialog openDlg(
					TRUE,
					( gzipAvailable ? "gz" : "sql" ),
					NULL,
					OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
					(
						gzipAvailable ?
						"Compressed SQL Files (*.gz)|*.gz|SQL Files (*.sql)|*.sql|All Files (*.*)|*.*||" :
						"SQL Files (*.sql)|*.sql|All Files (*.*)|*.*||"
					),
					mainWnd
				);

				if ( openDlg.DoModal() != IDCANCEL ) {
					string inputfile = openDlg.GetPathName();
					bool uncompressed = true;
					if ( openDlg.GetFileExt() == "gz" ) {
						char tempPath[10000];
						::GetTempPath( 9999, tempPath );
						string path = tempPath;
						if ( path[path.size()-1] != '\\' ) {
							path += '\\';
						}
						string file = (LPCTSTR)openDlg.GetFileName();
						file = file.substr( 0, file.size()-2 ) + "sql.gz";
						if ( !::CopyFile( inputfile.c_str(), (path+file).c_str(), false ) ) {
							uncompressed = false;
							CDialog::OnCancel();
						} else {
							inputfile = path + file;
							inputfile = inputfile.substr( 0, inputfile.length()-3 );
							remove( inputfile.c_str() );
							system( ( (string)"\"" + zipPath + "\" -d " + path + file ).c_str() );
						}
					}
					if ( uncompressed ) {
						handler.BeginRestore( inputfile.c_str(), &progress, &sql );
						timer = SetTimer( 1, 10, 0 );
					}
				} else {
					CDialog::OnCancel();
				}
			} else {
				CDialog::OnCancel();
			}
		} else {
			CDialog::OnCancel();
		}
	} else {
		CDialog::OnCancel();
	}

	return TRUE;
}

void CRestoreDlg::OnTimer( UINT nIDEvent ) {
	bool running = true;
	string message;
	for ( int i=0; i<200 && running; i++ ) {
		if ( !handler.Continue( message ) ) {
			running = false;
		}
	}
	if ( !running ) {
		KillTimer( timer );
		MessageBox( message.c_str(), "StudentInfo" );
		CDialog::OnCancel();
	}
	CDialog::OnTimer( nIDEvent );
}

void CRestoreDlg::OnCancel() {
	if ( MessageBox( "Are you sure you want to cancel this resore?", "StudentInfo", MB_YESNO ) == IDYES ) {
		KillTimer( timer );
		handler.Abort();
		CDialog::OnCancel();
	}
}

BEGIN_MESSAGE_MAP( CRestoreDlg, CDialog )
	ON_WM_TIMER()
END_MESSAGE_MAP()
