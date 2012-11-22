
#include "AssessmentModsButton.h"

CAssessmentModsButton::CAssessmentModsButton() {
	modified = false;
	readonly = false;
}

void CAssessmentModsButton::OnClick() {
	if ( readonly ) {
		vector<int> tempmods = mods;
		string tempdistrictAssessment = districtAssessment;
		bool tempmodified = modified;
		dlg.DoModal( &tempmods, &tempdistrictAssessment, &tempmodified, readonly );
	} else {
		dlg.DoModal( &mods, &districtAssessment, &modified, readonly );
		UpdateLabel();
	}
}

void CAssessmentModsButton::SetReadOnly( BOOL bReadOnly ) {
	readonly = bReadOnly ? true : false;
}

void CAssessmentModsButton::SetMods( vector<int> newMods ) {
	mods = newMods;
	UpdateLabel();
}

void CAssessmentModsButton::SetDistrictAssessment( string newDistrictAssessment ) {
	districtAssessment = newDistrictAssessment;
	UpdateLabel();
}

vector<int> CAssessmentModsButton::GetMods() {
	return mods;
}

string CAssessmentModsButton::GetDistrictAssessment() {
	return districtAssessment;
}

bool CAssessmentModsButton::IsModified() {
	return modified;
}

void CAssessmentModsButton::ResetModified() {
	modified = false;
}

void CAssessmentModsButton::UpdateLabel() {
	if ( mods.size() || districtAssessment.length() ) {
		SetWindowText( "Mods*" );
	} else {
		SetWindowText( "Mods" );
	}
}

IMPLEMENT_DYNAMIC( CAssessmentModsButton, CButton )

BEGIN_MESSAGE_MAP( CAssessmentModsButton, CButton )
	ON_CONTROL_REFLECT( BN_CLICKED, OnClick )
END_MESSAGE_MAP()
