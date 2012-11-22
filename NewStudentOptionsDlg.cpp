
#pragma warning(disable:4786)

#include "NewStudentOptionsDlg.h"
#include "MainWindow.h"
#include "StudentChooserWnd.h"
#include "StorageController.h"
#include "Preferences.h"
#include "resource.h"

extern CPreferences preferences;
extern CStorageController storage;
extern CMainWindow* mainWnd;

CNewStudentOptionsDlg::CNewStudentOptionsDlg() :CDialog(IDD_NEWSTUDENTOPTIONS) {
}

void CNewStudentOptionsDlg::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_IMPORTSSN, importSSN );
}

BOOL CNewStudentOptionsDlg::OnInitDialog() {
	if ( CDialog::OnInitDialog() ) {
		CheckDlgButton( IDC_CREATENEW, 1 );
		CheckDlgButton( IDC_SHOWNEWOPTIONS, 1 );
		return TRUE;
	}
	return FALSE;
}

void CNewStudentOptionsDlg::OnOK() {
	if ( !((CButton*)GetDlgItem(IDC_SHOWNEWOPTIONS))->GetCheck() ) {
		preferences.SetBool( "ShowNewStudentOptions", false );
	}
	if ( GetCheckedRadioButton( IDC_CREATENEW, IDC_IMPORT ) == IDC_IMPORT ) {
		importSSN.ValidateData();
		if ( storage.StudentExists( importSSN.GetString() ) ) {
			mainWnd->EditStudent( importSSN.GetString(), chooser->GetSchoolYearID() );
		} else {
			MessageBox( "No student exists with that SSN.", "Student Info" );
			return;
		}
	} else {
		chooser->CreateStudent();
	}
	CDialog::OnOK();
}

void CNewStudentOptionsDlg::OnCreateNewChosen() {
	GetDlgItem( IDC_IMPORTSSN )->EnableWindow( FALSE );
}

void CNewStudentOptionsDlg::OnImportChosen() {
	GetDlgItem( IDC_IMPORTSSN )->EnableWindow( TRUE );
}

BEGIN_MESSAGE_MAP( CNewStudentOptionsDlg, CDialog )
	ON_COMMAND( IDC_CREATENEW, OnCreateNewChosen )
	ON_COMMAND( IDC_IMPORT, OnImportChosen )
END_MESSAGE_MAP()