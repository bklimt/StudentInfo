
#include "ServicesPage.h"
#include "StudentEditDlg.h"

CServicesPage::CServicesPage() : CDialog( IDD_SERVICESPAGE ) {
}

void CServicesPage::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_INSTRUCTIONALSETTING, instructionalSetting );
	DDX_Control( pDX, IDC_INSTRUCTIONALSETTINGCODE, instructionalSettingCode );
	DDX_Control( pDX, IDC_LASTARDDATE, lastARD );
	DDX_Control( pDX, IDC_LASTANNUALREVIEWDATE, lastAnnualReview );
	DDX_Control( pDX, IDC_ITPDATE, itpDate );
	DDX_Control( pDX, IDC_IEPSTARTDATE, iepStartDate );
	DDX_Control( pDX, IDC_BIPDATE, bipDate );
	DDX_Control( pDX, IDC_IEPENDDATE, iepEndDate );
	DDX_Control( pDX, IDC_INSTRUCTIONALSERVICES, instructionalServices );
	DDX_Control( pDX, IDC_RELATEDSERVICES, relatedServices );
	DDX_Control( pDX, IDC_ESYSERVICES, esyServices );
	DDX_Control( pDX, IDC_SERVICESCOMMENTS, servicesComments );
	DDX_Control( pDX, IDC_MEDICALLYFRAGILE, medicallyFragile );
	DDX_Control( pDX, IDC_MEDICAID, medicaid );
}

BOOL CServicesPage::PreTranslateMessage( MSG* pMsg ) {
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

void CServicesPage::OnOK() {
	OnTabPressed();
}

void CServicesPage::OnCancel() {
}

void CServicesPage::UpdateInstructionalSettingCode() {
	instructionalSettingCode.SetIndex( instructionalSetting.GetSelectedIndex() );
}

bool CServicesPage::OnTabPressed() {
	if ( GetFocus() == &servicesComments ) {
		instructionalServices.BeginEdit( CGridLocation(0,0), NULL );
	} else {
		if ( GetFocus() == instructionalServices.GetCurrentControl() ) {
			relatedServices.BeginEdit( CGridLocation(0,0), NULL );
		} else {
			if ( GetFocus() == relatedServices.GetCurrentControl() ) {
				esyServices.BeginEdit( CGridLocation(0,0), NULL );
			} else {
				if ( GetFocus() == esyServices.GetCurrentControl() ) {
					CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
					parent->ShowNextPage( this );
				} else {
					return true;
				}
			}
		}
	}
	return false;
}

bool CServicesPage::OnShiftTabPressed() {
	if ( GetFocus() == &instructionalServices ) {
		servicesComments.SetFocus();
	} else {
		if ( GetFocus() == relatedServices.GetCurrentControl() ) {
			instructionalServices.BeginEdit( CGridLocation( instructionalServices.GetRowCount()-1, instructionalServices.GetColumnCount()-1 ), NULL );
		} else {
			if ( GetFocus() == esyServices.GetCurrentControl() ) {
				relatedServices.BeginEdit( CGridLocation( relatedServices.GetRowCount()-1, relatedServices.GetColumnCount()-1 ), NULL );
			} else {
				if ( GetFocus() == &instructionalSetting ) {
					CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
					parent->ShowPreviousPage( this );
				} else {
					return true;
				}
			}
		}
	}
	return false;
}

BEGIN_MESSAGE_MAP( CServicesPage, CDialog )
	ON_CBN_SELCHANGE( IDC_INSTRUCTIONALSETTING, UpdateInstructionalSettingCode )
END_MESSAGE_MAP()
