
#include "DemographicsPage.h"
#include "StudentEditDlg.h"

CDemographicsPage::CDemographicsPage() : CDialog( IDD_DEMOGRAPHICSPAGE ) {
}

void CDemographicsPage::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_COPYADDRESSUP   , copyUp      );
	DDX_Control( pDX, IDC_COPYADDRESSDOWN , copyDown    );
	DDX_Control( pDX, IDC_BIRTHDATE  , birthDate   );
	DDX_Control( pDX, IDC_CHRONAGE   , chronAge    );
	DDX_Control( pDX, IDC_ADDRESS    , address     );
	DDX_Control( pDX, IDC_CITY       , city        );
	DDX_Control( pDX, IDC_STATE      , state       );
	DDX_Control( pDX, IDC_ZIPCODE    , zipCode     );
	DDX_Control( pDX, IDC_MAILINGADDRESS    , mailingAddress     );
	DDX_Control( pDX, IDC_MAILINGCITY       , mailingCity        );
	DDX_Control( pDX, IDC_MAILINGSTATE      , mailingState       );
	DDX_Control( pDX, IDC_MAILINGZIPCODE    , mailingZipCode     );
	DDX_Control( pDX, IDC_HOMEPHONE  , homePhone   );
	DDX_Control( pDX, IDC_STATUSDATE , statusDate  );
	DDX_Control( pDX, IDC_STATUS     , status      );
	DDX_Control( pDX, IDC_ELIGIBILITY, eligibility );
	DDX_Control( pDX, IDC_GENDER     , gender      );
	DDX_Control( pDX, IDC_ETHNICITY  , ethnicity   );
	DDX_Control( pDX, IDC_CAMPUS     , campus      );
	DDX_Control( pDX, IDC_GRADE      , grade       );
	DDX_Control( pDX, IDC_DEMOGRAPHICSCOMMENTS, demographicsComments );
}

BOOL CDemographicsPage::PreTranslateMessage( MSG* pMsg ) {
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

void CDemographicsPage::OnOK() {
	OnTabPressed();
}

void CDemographicsPage::OnCancel() {
}

bool CDemographicsPage::OnTabPressed() {
	if ( GetFocus() == &demographicsComments ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowNextPage( this );
		return false;
	}
	return true;
}

bool CDemographicsPage::OnShiftTabPressed() {
	if ( GetFocus() == &birthDate ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowPreviousPage( this );
		return false;
	}
	return true;
}

void CDemographicsPage::OnBirthDateChanged() {
	int age = CDate::Today().GetYearDiff( birthDate.GetDate() );
	char temp[1000];
	sprintf( temp, "%d", age );
	chronAge.SetString( (age<0) ? "" : temp );
}

void CDemographicsPage::CopyAddressDown() {
	mailingAddress.SetString( address.GetString() );
	mailingCity.SetString( city.GetString() );
	mailingState.SetString( state.GetString() );
	mailingZipCode.SetString( zipCode.GetString() );
}

void CDemographicsPage::CopyAddressUp() {
	address.SetString( mailingAddress.GetString() );
	city.SetString( mailingCity.GetString() );
	state.SetString( mailingState.GetString() );
	zipCode.SetString( mailingZipCode.GetString() );
}

BEGIN_MESSAGE_MAP( CDemographicsPage, CDialog )
	ON_EN_CHANGE( IDC_BIRTHDATE, OnBirthDateChanged )
	ON_COMMAND( IDC_COPYADDRESSDOWN, CopyAddressDown )
	ON_COMMAND( IDC_COPYADDRESSUP, CopyAddressUp )
END_MESSAGE_MAP()
