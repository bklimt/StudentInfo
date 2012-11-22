
#include "AssessmentModsDlg.h"
#include "StorageController.h"
#include "resource.h"

extern CStorageController storage;

CAssessmentModsDlg::CAssessmentModsDlg() : CDialog( IDD_ASSESSMENTMODS ) {
}
	
void CAssessmentModsDlg::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_MODLIST, modsList );
	DDX_Control( pDX, IDC_DISTRICTASSESSMENT, district );
}

int CAssessmentModsDlg::DoModal( vector<int>* newMods, string* newDistrictAssessment, bool* newModified, bool newReadonly ) {
	mods = newMods;
	districtAssessment = newDistrictAssessment;
	modified = newModified;
	readonly = newReadonly;
	return CDialog::DoModal();
}

BOOL CAssessmentModsDlg::OnInitDialog() {
	BOOL retval = CDialog::OnInitDialog();

	CIndexedList list = storage.GetList( "AssessmentMod" );
	modsList.SetList( list );
	district.SetString( *districtAssessment );
	modsList.Select( *mods );

	modsList.SetReadOnly( readonly );
	district.SetReadOnly( readonly );

	return retval;
}

void CAssessmentModsDlg::OnOK() {
	*districtAssessment = district.GetString();
	*mods = modsList.GetSelectedIndices();
	*modified = district.IsModified() || modsList.IsModified();
	CDialog::OnOK();
}

