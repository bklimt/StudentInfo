
#ifndef STUDENTCHOOSERDLG_H
#define STUDENTCHOOSERDLG_H

#pragma warning(disable:4786)

#include <afxwin.h>
#include <afxext.h>
#include <map>
#include "StringEdit.h"
#include "SSNEdit.h"
#include "IndexedListBox.h"
using namespace std;

class CStudentChooserDlg : public CDialog {
	public:
		void DoDataExchange( CDataExchange* pDX );
		void OnOK();
		void OnCancel();

		void OnSize( UINT nType, int cx, int cy );
		void OnSetFocus( CWnd* pNewWnd );
		void OnTextChange();
		void OnSearchButtonClicked();
		void OnStudentListDoubleClicked();
		void OnNewStudentButtonClicked();
		void OnOpenStudentButtonClicked();
		void OnDeleteStudentButtonClicked();
		void OnSchoolYearSelected();

		void CreateStudent();
		int GetSchoolYearID();

		void RearrangeControls();
		void RefreshStudentList();
	private:
		CStringEdit searchFirstName;
		CStringEdit searchLastName;
		CSSNEdit    searchSSN;
		CButton     searchCheck;
		CListBox    studentList;
		CIndexedListBox schoolYearList;		

		map< CString, string > ssnLookup;

		CButton     newButton;
		CButton     openButton;
		CButton     searchButton;

		DECLARE_MESSAGE_MAP()
};

#endif
