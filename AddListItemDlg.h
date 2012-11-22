
/*
  AddListItemDlg.h - This object is the dialog box that lets
  the user add anew item to one of the combo box lists stored
  in the database
*/

#include <afxwin.h>      // for CDialog
#include "IndexedList.h" // the list to be edited is a CIndexedList

class CAddListItemDlg : public CDialog {
	public:
		// constructor
		CAddListItemDlg();
		// connect private data members to fields in the database
		void DoDataExchange( CDataExchange* pDX );
		// called whenever OK is pressed
		void OnOK();
		// called whenever the dialog is shown
		int DoModal( CIndexedList* newList );
	private:
		CIndexedList* list; // this is the list to be added to
		CString desc;       // this the string to be added to the list
		int id;             // this is the ID number to associate with the string
};
