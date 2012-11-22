
#include "DistributionsPage.h"
#include "StudentEditDlg.h"

CDistributionsPage::CDistributionsPage() : CDialog( IDD_DISTRIBUTIONSPAGE ) {
}

void CDistributionsPage::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_DISTRIBUTIONS, distributions );
	DDX_Control( pDX, IDC_DISTRIBUTIONSCOMMENTS, distributionsComments );
}

BOOL CDistributionsPage::PreTranslateMessage( MSG* pMsg ) {
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

void CDistributionsPage::OnOK() {
	OnTabPressed();
}

void CDistributionsPage::OnCancel() {
}

bool CDistributionsPage::OnTabPressed() {
	if ( GetFocus() == distributions.GetCurrentControl() ) {
		distributionsComments.SetFocus();
		return false;
	}
	if ( GetFocus() == &distributionsComments ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowNextPage( this );
		return false;
	}
	return true;
}

bool CDistributionsPage::OnShiftTabPressed() {
	if ( GetFocus() == &distributionsComments ) {
		distributions.BeginEdit( CGridLocation(distributions.GetRowCount()-1,distributions.GetColumnCount()-1), NULL );
		return false;
	}
	if ( GetFocus() == distributions.GetCurrentControl() ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowPreviousPage( this );
		return false;
	}
	return true;
}

BEGIN_MESSAGE_MAP( CDistributionsPage, CDialog )
END_MESSAGE_MAP()