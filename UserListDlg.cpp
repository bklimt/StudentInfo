
#include "UserListDlg.h"
#include "UserEditDlg.h"
#include "StorageController.h"
#include "resource.h"

extern CStorageController storage;

CUserListDlg::CUserListDlg() : CDialog( IDD_USERLIST ) {
}

BOOL CUserListDlg::OnInitDialog() {
	RefreshUserList();
	return CDialog::OnInitDialog();
}

void CUserListDlg::OnSize( UINT nType, int cx, int cy ) {
	CDialog::OnSize( nType, cx, cy );
	CListBox* userList = (CListBox*)GetDlgItem( IDC_USERLIST );
	CButton* newButton = (CButton*)GetDlgItem( IDC_NEWUSERBUTTON );
	CButton* deleteButton = (CButton*)GetDlgItem( IDC_DELETEUSERBUTTON );
	if ( userList && newButton && deleteButton ) {
		CRect client;
		GetClientRect( client );

		CRect button;
		newButton->CButton::GetWindowRect( button );

		userList->MoveWindow( 0, 0, client.Width(), client.Height()-button.Height() );
		newButton->MoveWindow( 0, client.Height()-button.Height(), client.Width()/2, button.Height() );
		deleteButton->MoveWindow( client.Width()/2, client.Height()-button.Height(), client.Width()/2, button.Height() );
	}
}

void CUserListDlg::OnUserListDblClk() {
	CString username;
	CUserEditDlg dialog;
	CListBox* userList = (CListBox*)GetDlgItem( IDC_USERLIST );
	if ( userList ) {
		if ( userList->GetCurSel() >= 0 ) {
			userList->GetText( userList->GetCurSel(), username );
			dialog.DoModal( (LPCTSTR)username );
			RefreshUserList();
		}
	}
}

void CUserListDlg::OnNewUserButton() {
	CUserEditDlg dialog;
	dialog.DoModal( "" );
	RefreshUserList();
}

void CUserListDlg::OnDeleteUserButton() {
	CString username;
	CListBox* userList = (CListBox*)GetDlgItem( IDC_USERLIST );
	if ( userList ) {
		if ( userList->GetCurSel() >= 0 ) {
			userList->GetText( userList->GetCurSel(), username );
			char temp[10000];
			sprintf( temp, "Are you sure you want to delete the user %s?", (LPCTSTR)username );
			if ( MessageBox( temp, "Student Info", MB_YESNO ) == IDYES ) {
				storage.DeleteUser( (LPCTSTR)username );
				RefreshUserList();
			}
		}
	}
}

void CUserListDlg::RefreshUserList() {
	vector<string> userList = storage.GetUserList();
	CListBox* userListBox = (CListBox*)GetDlgItem( IDC_USERLIST );
	if ( userListBox ) {
		userListBox->ResetContent();
		for ( int i=0; i<userList.size(); i++ ) {
			userListBox->AddString( userList[i].c_str() );
		}
	}
}

BEGIN_MESSAGE_MAP( CUserListDlg, CDialog )
	ON_WM_SIZE()
	ON_LBN_DBLCLK( IDC_USERLIST, OnUserListDblClk )
	ON_COMMAND( IDC_NEWUSERBUTTON, OnNewUserButton )
	ON_COMMAND( IDC_DELETEUSERBUTTON, OnDeleteUserButton )
END_MESSAGE_MAP()