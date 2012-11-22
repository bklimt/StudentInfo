
#include "EvaluationsPage.h"
#include "StudentEditDlg.h"

CEvaluationsPage::CEvaluationsPage() : CDialog( IDD_EVALUATIONSPAGE ) {
}

void CEvaluationsPage::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_PRIMARYDISABILITY,       primaryDisability       );
	DDX_Control( pDX, IDC_SECONDARYDISABILITY,     secondaryDisability     );
	DDX_Control( pDX, IDC_TERTIARYDISABILITY,      tertiaryDisability      );
	DDX_Control( pDX, IDC_PRIMARYDISABILITYCODE,   primaryDisabilityCode   );
	DDX_Control( pDX, IDC_SECONDARYDISABILITYCODE, secondaryDisabilityCode );
	DDX_Control( pDX, IDC_TERTIARYDISABILITYCODE,  tertiaryDisabilityCode  );
	DDX_Control( pDX, IDC_EVALUATIONS,             evaluations             );
	DDX_Control( pDX, IDC_EVALUATIONSCOMMENTS,     evaluationsComments     );
	DDX_Control( pDX, IDC_MULTIPLEDISABILITIES,    multipleDisabilities    );
}

void CEvaluationsPage::UpdateDisabilityCodes() {
	primaryDisabilityCode.SetIndex( primaryDisability.GetSelectedIndex() );
	secondaryDisabilityCode.SetIndex( secondaryDisability.GetSelectedIndex() );
	tertiaryDisabilityCode.SetIndex( tertiaryDisability.GetSelectedIndex() );
}

BOOL CEvaluationsPage::PreTranslateMessage( MSG* pMsg ) {
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
	} else {
		return CDialog::PreTranslateMessage( pMsg );
	}
}

void CEvaluationsPage::OnOK() {
	OnTabPressed();
}

void CEvaluationsPage::OnCancel() {
}

bool CEvaluationsPage::OnTabPressed() {
	if ( GetFocus() == evaluations.GetCurrentControl() ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowNextPage( this );
		return false;
	}
	if ( GetFocus() == &evaluationsComments ) {
		evaluations.BeginEdit( CGridLocation(0,0), NULL );
		return false;
	}
	return true;
}

bool CEvaluationsPage::OnShiftTabPressed() {
	if ( GetFocus() == evaluations.GetCurrentControl() ) {
		evaluationsComments.SetFocus();
		return false;
	}
	if ( GetFocus() == &primaryDisability ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowPreviousPage( this );
		return false;
	}
	return true;
}

BEGIN_MESSAGE_MAP( CEvaluationsPage, CDialog )
	ON_CBN_SELCHANGE( IDC_PRIMARYDISABILITY,   UpdateDisabilityCodes )
	ON_CBN_SELCHANGE( IDC_SECONDARYDISABILITY, UpdateDisabilityCodes )
	ON_CBN_SELCHANGE( IDC_TERTIARYDISABILITY,  UpdateDisabilityCodes )
END_MESSAGE_MAP()
