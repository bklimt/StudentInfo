
#pragma warning(disable:4786)
#include <afxdlgs.h>
#include "resource.h"
#include "BackupDlg.h"
#include "MainWindow.h"
#include "StorageController.h"
#include "Preferences.h"

extern CPreferences preferences;
extern CMainWindow* mainWnd;

CBackupDlg::CBackupDlg() : CDialog( IDD_BACKUP ) {
//	running = false;
//	finished = false;
}

void CBackupDlg::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_BACKUPPROGRESS, progress );
	DDX_Control( pDX, IDC_BACKUPTABLELIST, tableList );
}

BOOL CBackupDlg::OnInitDialog() {
	if ( !CDialog::OnInitDialog() ) {
		return FALSE;
	}

	zipPath = preferences.GetHomePath() + "gzip.exe";
	bool gzipAvailable = ( GetFileAttributes( zipPath.c_str() ) != -1 );

	CFileDialog saveDlg(
		FALSE,
		( gzipAvailable ? "gz" : "sql" ),
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(
			gzipAvailable ?
			"Compressed SQL File (*.gz)|*.gz|SQL File (*.sql)|*.sql|All Files (*.*)|*.*||" :
			"SQL File (*.sql)|*.sql|All Files (*.*)|*.*||"
		),
		mainWnd
	);

	if ( saveDlg.DoModal() != IDCANCEL ) {
		outfile = saveDlg.GetPathName();
		if ( saveDlg.GetFileExt() == "gz" ) {
			compress = true;
		} else {
			compress = false;
		}

		if ( compress ) {
			tempfile = (LPCTSTR)saveDlg.GetFileName();
			tempfile = tempfile.substr( 0, tempfile.length()-3 );
			char tempPath[10000];
			::GetTempPath( 9999, tempPath );
			string path = tempPath;
			if ( path[path.length()-1] != '\\' ) {
				path += '\\';
			}

			try { remove( ( path + tempfile + ".sql.gz" ).c_str() ); }
			catch (...) {}

			tempfile = path + tempfile + ".sql";
		}

		vector<string> tables;
		tables.push_back( "Assessment" );
		tables.push_back( "Campus" );
		tables.push_back( "Gender" );
		tables.push_back( "Ethnicity" );
		tables.push_back( "Status" );
		tables.push_back( "Eligibility" );
		tables.push_back( "Grade" );
		tables.push_back( "Disability" );
		tables.push_back( "SchoolYear" );
		tables.push_back( "AssessmentMod" );
		tables.push_back( "InstructionalSetting" );
		tables.push_back( "EvaluationEligible" );
		tables.push_back( "ReferralEligible" );
		tables.push_back( "ServiceType" );
		tables.push_back( "EvaluationType" );
		tables.push_back( "InstructionalServiceType" );
		tables.push_back( "RelatedServiceType" );
		tables.push_back( "ESYServiceType" );
		tables.push_back( "AchLevel" );
		tables.push_back( "Document" );
		tables.push_back( "GradeLevel" );
		tables.push_back( "DistributionMethod" );
		tables.push_back( "InitialDistribution" );
		tables.push_back( "BaselineYear" );
		tables.push_back( "ARDExpectation" );

		tables.push_back( "Person" );
		tables.push_back( "AssessmentRecord" );
		tables.push_back( "Student" );
		tables.push_back( "StudentContact" );
		tables.push_back( "StudentYear" );
		tables.push_back( "Evaluation" );
		tables.push_back( "InstructionalService" );
		tables.push_back( "RelatedService" );
		tables.push_back( "ESYService" );
		tables.push_back( "StudentEvaluationRequest" );
		tables.push_back( "AssessmentRecordMod" );
		tables.push_back( "Distribution" );

		CImageList* imageList = new CImageList();
		imageList->CImageList::Create( 16, 16, ILC_COLOR, 2, 2 );
		imageList->Add( AfxGetApp()->LoadIcon( IDI_NOCHECK ) );
		imageList->Add( AfxGetApp()->LoadIcon( IDI_CHECK ) );
		tableList.SetImageList( imageList, LVSIL_SMALL );

		for ( int i=0; i<tables.size(); i++ ) {
			tableList.InsertItem( i, tables[i].c_str(), 0 );
		}

		if ( compress ) {
			handler.BeginBackup( tables, tempfile.c_str(), &progress );
		} else {
			handler.BeginBackup( tables, outfile.c_str(), &progress );
		}
		timer = SetTimer( 1, 10, 0 );
	} else {
		CDialog::OnCancel();
	}

	return TRUE;
}

void CBackupDlg::OnTimer( UINT nIDEvent ) {
	string message;
	if ( !handler.Continue( message ) ) {
		KillTimer( timer );
		if ( compress && handler.WasSuccessful() ) {
			system( ( (string)"\"" + zipPath + "\" " + tempfile ).c_str() );
			if ( !::CopyFile( ( tempfile + ".gz" ).c_str(), outfile.c_str(), false ) ) {
				message = "Unable to create backup file!";
			}
		}
		if ( !compress && !handler.WasSuccessful() ) {
			try { remove( outfile.c_str() ); }
			catch (...) {}
		}
		MessageBox( message.c_str() );
		CDialog::OnCancel();
	} else {
		int current = handler.GetCurrentTable();
		for ( int i=0; i<current; i++ ) {
			tableList.CListCtrl::SetItem( i, 0, LVIF_IMAGE, NULL, 1, NULL, NULL, NULL );
		}
	}
	CDialog::OnTimer( nIDEvent );
}

void CBackupDlg::OnCancel() {
	if ( MessageBox( "Are you sure you want to cancel this backup?", "StudentInfo", MB_YESNO ) == IDYES ) {
		KillTimer( timer );
		handler.Abort();
		if ( !compress ) {
			try { remove( outfile.c_str() ); }
			catch (...) {}
		}
		CDialog::OnCancel();
	}
}

BEGIN_MESSAGE_MAP( CBackupDlg, CDialog )
	ON_WM_TIMER()
END_MESSAGE_MAP()
