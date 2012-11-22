
#include "ContactsPage.h"
#include "StudentEditDlg.h"

CContactsPage::CContactsPage() : CDialog( IDD_CONTACTSPAGE ) {
}

void CContactsPage::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_MOTHERFIRSTNAME, motherFirstName );
	DDX_Control( pDX, IDC_MOTHERLASTNAME,  motherLastName  );
	DDX_Control( pDX, IDC_MOTHERADDRESS,   motherAddress   );
	DDX_Control( pDX, IDC_MOTHERCITY,      motherCity      );
	DDX_Control( pDX, IDC_MOTHERSTATE,     motherState     );
	DDX_Control( pDX, IDC_MOTHERZIPCODE,   motherZipCode   );
	DDX_Control( pDX, IDC_MOTHERWORKPHONE, motherWorkPhone );
	DDX_Control( pDX, IDC_MOTHERHOMEPHONE, motherHomePhone );
	DDX_Control( pDX, IDC_MOTHERCELLPHONE, motherCellPhone );
	DDX_Control( pDX, IDC_MOTHEREMAIL,     motherEmail     );

	DDX_Control( pDX, IDC_FATHERFIRSTNAME, fatherFirstName );
	DDX_Control( pDX, IDC_FATHERLASTNAME,  fatherLastName  );
	DDX_Control( pDX, IDC_FATHERADDRESS,   fatherAddress   );
	DDX_Control( pDX, IDC_FATHERCITY,      fatherCity      );
	DDX_Control( pDX, IDC_FATHERSTATE,     fatherState     );
	DDX_Control( pDX, IDC_FATHERZIPCODE,   fatherZipCode   );
	DDX_Control( pDX, IDC_FATHERWORKPHONE, fatherWorkPhone );
	DDX_Control( pDX, IDC_FATHERHOMEPHONE, fatherHomePhone );
	DDX_Control( pDX, IDC_FATHERCELLPHONE, fatherCellPhone );
	DDX_Control( pDX, IDC_FATHEREMAIL,     fatherEmail     );

	DDX_Control( pDX, IDC_CONTACTSCOMMENTS, contactsComments );
}

BOOL CContactsPage::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN && (char)(pMsg->wParam) == '\t' ) {
		if ( GetKeyState( VK_SHIFT ) & 1<<31 ) {
			if ( OnShiftTabPressed() ) {
				return CDialog::PreTranslateMessage( pMsg );
			} else {
				return TRUE;
			}
		} else {
			if ( OnTabPressed() ) {
				return CDialog::PreTranslateMessage( pMsg );
			} else {
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage( pMsg );
}

void CContactsPage::OnOK() {
	OnTabPressed();
}

void CContactsPage::OnCancel() {
}

bool CContactsPage::OnTabPressed() {
	if ( GetFocus() == &contactsComments ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowNextPage( this );
		return false;
	}
	return true;
}

bool CContactsPage::OnShiftTabPressed() {
	if ( GetFocus() == &motherFirstName ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowPreviousPage( this );
		return false;
	}
	return true;
}
