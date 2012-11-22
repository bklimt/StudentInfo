
#include "UserEditDlg.h"
#include "StorageController.h"
#include "resource.h"

extern CStorageController storage;

CUserEditDlg::CUserEditDlg() : CDialog( IDD_USEREDIT ) {
}

BOOL CUserEditDlg::OnInitDialog() {
	if ( CDialog::OnInitDialog() ) {
		SetDlgItemText( IDC_USERNAME, editUser.GetUsername().c_str() );
		SetDlgItemText( IDC_PASSWORD1, editUser.GetPassword().c_str() );
		SetDlgItemText( IDC_PASSWORD2, editUser.GetPassword().c_str() );
		//CheckDlgButton( IDC_ISADMIN, ( editUser.IsAdmin() ? 1 : 0 ) );
		CheckDlgButton( IDC_CANWRITE, ( editUser.CanWrite() ? 1 : 0 ) );

		if ( editUser.GetUsername() == "admin" ) {
			( (CButton*)GetDlgItem( IDC_USERNAME ) )->EnableWindow( FALSE );
			//( (CButton*)GetDlgItem( IDC_ISADMIN ) )->EnableWindow( FALSE );
			( (CButton*)GetDlgItem( IDC_CANWRITE ) )->EnableWindow( FALSE );
		}

		return TRUE;
	}
	return FALSE;
}

void CUserEditDlg::OnOK() {
	CUser newUser;
	CString username;
	CString password1;
	CString password2;
	//int isAdmin;
	int canWrite;

	GetDlgItemText( IDC_USERNAME, username );
	GetDlgItemText( IDC_PASSWORD1, password1 );
	GetDlgItemText( IDC_PASSWORD2, password2 );
	//isAdmin = ((CButton*)GetDlgItem( IDC_ISADMIN ))->GetCheck();
	canWrite = ((CButton*)GetDlgItem( IDC_CANWRITE ))->GetCheck();

	if ( password1 != password2 ) {
		MessageBox( "The passwords given do not match.", "StudentInfo" );
		return;
	}

	if ( editUser.GetUsername() != (LPCTSTR)username ) {
		if ( storage.GetUser( (LPCTSTR)username ).IsValid() ) {
			MessageBox( "Another user already has this username.", "StudentInfo" );
			return;
		}
	}

	newUser.SetUsername( (LPCTSTR)username );
	newUser.SetPassword( (LPCTSTR)password1 );
	//newUser.SetIsAdmin( ( isAdmin == 0 ? false : true ) );
	newUser.SetCanWrite( ( canWrite == 0 ? false : true ) );

	if ( !editUser.IsValid() ) {
		if ( !storage.CreateUser( (LPCTSTR)username ) ) {
			return;
		}
		editUser.SetUsername( (LPCTSTR)username );
	}

	if ( !storage.UpdateUser( editUser.GetUsername(), newUser ) ) {
		return;
	}

	CDialog::OnOK();
}

void CUserEditDlg::DoModal( string username ) {
	if ( username == "" ) {
		editUser = CUser();
	} else {
		editUser = storage.GetUser( username );
	}
	CDialog::DoModal();
}
