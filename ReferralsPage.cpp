
#include "ReferralsPage.h"
#include "StudentEditDlg.h"

CReferralsPage::CReferralsPage() : CDialog( IDD_REFERRALSPAGE ) {
}

void CReferralsPage::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_REFERRALDATE            , referralDate            );
	DDX_Control( pDX, IDC_REFERRINGCAMPUS         , referringCampus         );
	DDX_Control( pDX, IDC_EVALUATIONDUEDATE       , evaluationDueDate       );
	DDX_Control( pDX, IDC_ARDDUEDATE              , ardDueDate              );
	DDX_Control( pDX, IDC_EVALUATIONCOMPLETEDDATE , evaluationCompletedDate );
	DDX_Control( pDX, IDC_ARDCOMPLETEDDATE        , ardCompletedDate        );
	DDX_Control( pDX, IDC_REFERRALELIGIBLE        , referralEligible        );
	DDX_Control( pDX, IDC_EVALUATIONREQUESTLIST   , evaluationRequestList   );
	DDX_Control( pDX, IDC_EVALUATIONSREQUESTED    , evaluationsRequested    );
	DDX_Control( pDX, IDC_REFERRALSCOMMENTS       , referralsComments       );
	DDX_Control( pDX, IDC_ADDEVALUATION           , addEvaluationButton     );
	DDX_Control( pDX, IDC_REMOVEEVALUATION        , removeEvaluationButton  );
}

BOOL CReferralsPage::PreTranslateMessage( MSG* pMsg ) {
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

void CReferralsPage::OnOK() {
	OnTabPressed();
}

void CReferralsPage::OnCancel() {
}

void CReferralsPage::OnAddEvaluation() {
	int index = evaluationRequestList.GetCurSel();
	if ( index >= 0 ) {
		CString str;
		evaluationRequestList.GetLBText( index, str );
		if ( str != "" ) {
			bool found = false;
			for ( int i=0; i<evaluationsRequested.GetCount() && !found; i++ ) {
				CString item;
				evaluationsRequested.GetText( i, item );
				if ( str == item ) {
					found = true;
				}
			}
			if ( !found ) {
				evaluationsRequested.AddString( str );
				CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
				parent->Modify();
			}
		}
	}
}

void CReferralsPage::OnRemoveEvaluation() {
	for ( int i=evaluationsRequested.GetCount()-1; i>=0; i-- ) {
		if ( evaluationsRequested.GetSel( i ) ) {
			evaluationsRequested.DeleteString( i );
			CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
			parent->Modify();
		}
	}
}

bool CReferralsPage::OnTabPressed() {
	if ( GetFocus() == &referralsComments ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowNextPage( this );
		return false;
	}
	return true;
}

bool CReferralsPage::OnShiftTabPressed() {
	if ( GetFocus() == &referralDate ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowPreviousPage( this );
		return false;
	}
	return true;
}

void CReferralsPage ::OnReferralDateChanged() {
	evaluationDueDate.SetDate( referralDate.GetDate()+60 );
	OnEvaluationCompletedDateChanged();
}

void CReferralsPage::OnEvaluationCompletedDateChanged() {
	if ( evaluationCompletedDate.GetDate().IsNull() ) {
		ardDueDate.SetDate( evaluationDueDate.GetDate()+30 );
	} else {
		ardDueDate.SetDate( evaluationCompletedDate.GetDate()+30 );
	}
}

BEGIN_MESSAGE_MAP( CReferralsPage, CDialog )
	ON_COMMAND( IDC_ADDEVALUATION, OnAddEvaluation )
	ON_COMMAND( IDC_REMOVEEVALUATION, OnRemoveEvaluation )
	ON_EN_CHANGE( IDC_REFERRALDATE, OnReferralDateChanged )
	ON_EN_CHANGE( IDC_EVALUATIONCOMPLETEDDATE, OnEvaluationCompletedDateChanged )
END_MESSAGE_MAP()