
#include "LoginDlg.h"
#include "User.h"
#include "StorageController.h"
#include "Preferences.h"
#include "ConnectionOptionsDlg.h"
#include "resource.h"

extern CUser user;
extern CPreferences preferences;
extern CStorageController storage;

CLoginDlg::CLoginDlg() : CDialog( IDD_LOGIN ) {
}

BOOL CLoginDlg::OnInitDialog() {
	if ( !CDialog::OnInitDialog() ) {
		return FALSE;
	}
	SetDlgItemText( IDC_DATABASE, preferences.GetString( "DSN" ).c_str() );
	/*{
		char temp[1000];
		//sprintf( temp, "%d,%d", LOWORD(::GetDialogBaseUnits()), HIWORD(::GetDialogBaseUnits()) );
		//MessageBox(0,temp,"",0);
		CRect area(0,0,12,12);
		MapDialogRect( area );
		sprintf(temp,"%d",area.Height());
		MessageBox(temp);
	}*/
	return TRUE;
}

void CLoginDlg::OnShowWindow( BOOL bShow, UINT nStatus ) {
	CDialog::OnShowWindow( bShow, nStatus );
	( (CEdit*)GetDlgItem( IDC_USERNAME ) )->SetFocus();
}
  
void CLoginDlg::OnOK() {
	CString username, password, database;

	GetDlgItemText( IDC_USERNAME, username );
	GetDlgItemText( IDC_PASSWORD, password );
	GetDlgItemText( IDC_DATABASE, database );

	preferences.SetString( "DSN", (LPCTSTR)database );

	if ( storage.ValidateUser( (LPCTSTR)username, (LPCTSTR)password ) ) {
		CDialog::OnOK();
	} else {
		SetDlgItemText( IDC_USERNAME, "" );
		SetDlgItemText( IDC_PASSWORD, "" );
		( (CEdit*)GetDlgItem( IDC_USERNAME ) )->SetFocus();
	}
}

void CLoginDlg::OnCancel() {
	exit(-1);
}

void CLoginDlg::OnOptionsClicked() {
	CConnectionOptionsDlg dlg;
	if ( dlg.DoModal() == IDOK ) {
		SetDlgItemText( IDC_DATABASE, preferences.GetString( "DSN" ).c_str() );
	}
}

BEGIN_MESSAGE_MAP( CLoginDlg, CDialog )
	ON_WM_SHOWWINDOW()
	ON_COMMAND( IDC_OPTIONS, OnOptionsClicked )
END_MESSAGE_MAP()
