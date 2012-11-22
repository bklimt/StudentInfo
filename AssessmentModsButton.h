
#include "AssessmentModsDlg.h"

class CAssessmentModsButton : public CButton {
	public:
		CAssessmentModsButton();
		void OnClick();
		void SetReadOnly( BOOL bReadOnly = TRUE );

		void SetMods( vector<int> newMods );
		void SetDistrictAssessment( string newDistrictAssessment );

		vector<int> GetMods();
		string GetDistrictAssessment();

		virtual bool IsModified();
		virtual void ResetModified();

		void UpdateLabel();
	private:
		vector<int> mods;
		string districtAssessment;

		bool readonly;
		bool modified;
		CAssessmentModsDlg dlg;
		DECLARE_DYNAMIC( CAssessmentModsButton );
		DECLARE_MESSAGE_MAP()
};