
#include <afxwin.h>
#include "SSNEdit.h"

class CNewStudentOptionsDlg : public CDialog {
	public:
		CNewStudentOptionsDlg();
		void DoDataExchange( CDataExchange* pDX );
		BOOL OnInitDialog();
		void OnOK();

		void OnCreateNewChosen();
		void OnImportChosen();

	private:
		CSSNEdit importSSN;

		DECLARE_MESSAGE_MAP()
};
