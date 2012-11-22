
#pragma warning(disable:4786)

#include <afxwin.h>
#include <string>
#include <vector>
#include "StringEdit.h"
#include "CheckIndexedListBox.h"
using namespace std;

class CAssessmentModsDlg : public CDialog {
	public:
		CAssessmentModsDlg();
		// connect private data members to fields in the database
		void DoDataExchange( CDataExchange* pDX );
		int DoModal( vector<int>* newMods, string* newDistrictAssessment, bool* newModified, bool newReadonly );
		BOOL OnInitDialog();
		void OnOK();
	private:
		vector<int>* mods;
		string* districtAssessment;
		bool* modified;
		bool readonly;
		CCheckIndexedListBox modsList;
		CStringEdit district;
};
