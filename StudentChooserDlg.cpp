
#pragma warning(disable:4786)

#include "MainWindow.h"
#include "StudentChooserDlg.h"
#include "StorageController.h"
#include "NewStudentOptionsDlg.h"
#include "Preferences.h"
#include "resource.h"

extern CStorageController storage;
extern CUser user;
extern CPreferences preferences;
extern CMainWindow* mainWnd;

void CStudentChooserDlg::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_SEARCHFIRSTNAME,  searchFirstName );
	DDX_Control( pDX, IDC_SEARCHLASTNAME,   searchLastName );
	DDX_Control( pDX, IDC_SEARCHSSN,        searchSSN );
	DDX_Control( pDX, IDC_SEARCHCHECK,      searchCheck );
	DDX_Control( pDX, IDC_STUDENTLIST,      studentList );
	DDX_Control( pDX, IDC_NEWSTUDENT,       newButton );
	DDX_Control( pDX, IDC_OPENSTUDENT,      openButton );
	DDX_Control( pDX, IDC_SEARCHBUTTON,     searchButton );
	DDX_Control( pDX, IDC_SEARCHSCHOOLYEAR, schoolYearList );

	searchCheck.SetCheck( 1 );

	schoolYearList.DisableNull();
	schoolYearList.SetList( storage.GetSchoolYearList() );

	RefreshStudentList();

	searchLastName.SetFocus();

	if ( !user.CanWrite() ) {
		newButton.EnableWindow( FALSE );
	}

	try {
		schoolYearList.Select( preferences.GetInt( "CurrentSchoolYear" ) );
		RefreshStudentList();
	}
	catch (...) {}
}

void CStudentChooserDlg::OnOK() {
}

void CStudentChooserDlg::OnCancel() {
}

void CStudentChooserDlg::OnSize( UINT nType, int cx, int cy ) {
	if ( GetDlgItem( IDC_SEARCHFIRSTNAME ) ) {
		RearrangeControls();
	}
	CDialog::OnSize( nType, cx, cy );
}

void CStudentChooserDlg::OnSetFocus( CWnd* pNewWnd ) {
	CDialog::OnSetFocus( pNewWnd );
	searchLastName.SetFocus();
}

void CStudentChooserDlg::OnTextChange() {
	if ( searchCheck.GetCheck() ) {
		OnSearchButtonClicked();
	}
}

void CStudentChooserDlg::OnSearchButtonClicked() {
	RefreshStudentList();
	for ( int i=0; i<studentList.GetCount(); i++ ) {
		studentList.SetSel( i, FALSE );
	}
	if ( studentList.GetCount() > 0 ) {
		studentList.SetSel( 0 );
	}
}

void CStudentChooserDlg::OnStudentListDoubleClicked() {
	if ( studentList.GetCurSel() >= 0 ) {
		CString temp;
		studentList.GetText( studentList.GetCurSel(), temp );
		mainWnd->EditStudent( ssnLookup[temp], schoolYearList.GetSelectedIndex() );
		/*
		string ssn = (LPCTSTR)temp;
		if ( ssn.length() > 0 ) {
			ssn = ssn.substr( ssn.length()-12, 11 );
			mainWnd->EditStudent( ssn, schoolYearList.GetSelectedIndex() );
		}
		*/
	}
}

void CStudentChooserDlg::OnNewStudentButtonClicked() {
	if ( preferences.GetBool( "ShowNewStudentOptions" ) ) {
		CNewStudentOptionsDlg dialog;
		dialog.DoModal();
	} else {
		mainWnd->NewStudent( schoolYearList.GetSelectedIndex() );
	}
}

void CStudentChooserDlg::OnOpenStudentButtonClicked() {
	for ( int i=0; i<studentList.GetCount(); i++ ) {
		if ( studentList.GetSel( i ) ) {
			CString temp;
			studentList.GetText( i, temp );
			mainWnd->EditStudent( ssnLookup[temp], schoolYearList.GetSelectedIndex() );
			/*
			string ssn = (LPCTSTR)temp;
			if ( ssn.length() > 0 ) {
				ssn = ssn.substr( ssn.length()-12, 11 );
				mainWnd->EditStudent( ssn, schoolYearList.GetSelectedIndex() );
			}
			*/
		}
	}
}

void CStudentChooserDlg::OnDeleteStudentButtonClicked() {
	for ( int i=0; i<studentList.GetCount(); i++ ) {
		if ( studentList.GetSel( i ) ) {
			CString temp;
			studentList.GetText( i, temp );
			HRESULT ans = MessageBox( "Are you sure you want to delete "+temp+"?", "", MB_YESNO );
			if ( ans == IDYES ) {
				storage.DeleteStudentYear( ssnLookup[temp], schoolYearList.GetSelectedIndex() );
			}
		}
	}
	/*if ( studentList.GetCurSel() >= 0 ) {
		HRESULT ans = MessageBox( "Are you sure you want to delete this student?", "", MB_YESNO );
		if ( ans == IDYES ) {
			CString temp;
			studentList.GetText( studentList.GetCurSel(), temp );
			string ssn = (LPCTSTR)temp;
			if ( ssn.length() > 12 ) {
				ssn = ssn.substr( ssn.length()-12, 11 );
				storage.DeleteStudentYear( ssn, schoolYearList.GetSelectedIndex() );
			}
		}
	}*/
	RefreshStudentList();
}

void CStudentChooserDlg::OnSchoolYearSelected() {
	RefreshStudentList();
}

void CStudentChooserDlg::RearrangeControls() {
	RECT temp;
	GetClientRect( &temp );
	int cx = temp.right;
	int cy = temp.bottom;

	newButton.GetWindowRect( &temp );
	int buttonHeight = temp.bottom - temp.top;
	int buttonWidth = ( cx - 30 ) / 2;
	searchFirstName.GetWindowRect( &temp );
	int editHeight = temp.bottom - temp.top;
	searchCheck.GetWindowRect( &temp );
	int checkWidth = temp.right - temp.left;

	newButton.MoveWindow( 10, cy-buttonHeight-10, buttonWidth, buttonHeight );
	openButton.MoveWindow( buttonWidth+20, cy-buttonHeight-10, buttonWidth, buttonHeight );

	studentList.MoveWindow( 10, 132, cx-20, cy-buttonHeight-40-112 );

	searchLastName.MoveWindow( 70, 12, cx - 80, editHeight );
	searchFirstName.MoveWindow( 70, 34, cx - 80, editHeight );
	searchSSN.MoveWindow( 70, 56, cx - 80, editHeight );
	schoolYearList.MoveWindow( 70, 78, cx - 80, editHeight );

	searchCheck.MoveWindow( cx-checkWidth-10, 102, checkWidth, 30 );
	searchButton.MoveWindow( 10, 104, cx-checkWidth-30, buttonHeight );
}

void CStudentChooserDlg::RefreshStudentList() {
	vector< CString > selected;
	for ( int i=0; i<studentList.GetCount(); i++ ) {
		if ( studentList.GetSel( i ) ) {
			CString text;
			studentList.GetText( i, text );
			selected.push_back( text );
		}
	}

	CTable table;
	storage.GetStudentList(
		table,
		searchFirstName.GetString(),
		searchLastName.GetString(),
		searchSSN.GetString(),
		schoolYearList.GetSelectedIndex()
	);

	int index = 0;
	studentList.ResetContent();
	ssnLookup.clear();
	while ( !table.IsAtEOF() ) {
		string entry;
		if ( (string) table["LastName"] == "" || (string) table["FirstName"] == "" ) {
			entry = (string) "[" + (string) table["SSN"] + "]";
		} else {
			entry =
				(string) table["LastName"] + ", " +
				(string) table["FirstName"] + " [" +
				(string) table["SSN"] + "]";
		}
		studentList.AddString( entry.c_str() );
		ssnLookup[ entry.c_str() ] = table["SSN"];
		table.MoveNext();
	}

	for ( i=0; i<studentList.GetCount(); i++ ) {
		CString text;
		studentList.GetText( i, text );
		for ( int j=0; j<selected.size(); j++ ) {
			if ( selected[j] == text ) {
				studentList.SetSel( i );
			}
		}
	}
}

void CStudentChooserDlg::CreateStudent() {
	mainWnd->NewStudent( schoolYearList.GetSelectedIndex() );
}

int CStudentChooserDlg::GetSchoolYearID() {
	return schoolYearList.GetSelectedIndex();
}

BEGIN_MESSAGE_MAP( CStudentChooserDlg, CDialog )
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_EN_CHANGE( IDC_SEARCHFIRSTNAME, OnTextChange )
	ON_EN_CHANGE( IDC_SEARCHLASTNAME, OnTextChange )
	ON_EN_CHANGE( IDC_SEARCHSSN, OnTextChange )
	ON_BN_CLICKED( IDC_SEARCHBUTTON, OnSearchButtonClicked )
	ON_BN_CLICKED( IDC_NEWSTUDENT, OnNewStudentButtonClicked )
	ON_BN_CLICKED( IDC_OPENSTUDENT, OnOpenStudentButtonClicked )
	ON_LBN_DBLCLK( IDC_STUDENTLIST, OnStudentListDoubleClicked )
	ON_CBN_SELCHANGE( IDC_SEARCHSCHOOLYEAR, OnSchoolYearSelected )
END_MESSAGE_MAP()
