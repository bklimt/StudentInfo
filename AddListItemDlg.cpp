
/*
  AddListItemDlg.cpp - This is the implemantation of the dialog that
  lets the user add a new item to one of the combo box lists.
*/

#include "AddListItemDlg.h"
#include "StorageController.h" // to add the new item to the database
#include "resource.h"          // for the dialog's template

extern CStorageController storage;

// constructor
CAddListItemDlg::CAddListItemDlg() : CDialog( IDD_ADDLISTITEM ) { // load the template from the resource
	// set default values for the new list item
	id = 0;
	desc = "";
}

// link the private members to the fields in the dialog
void CAddListItemDlg::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	// link the strings to the edit boxes
	DDX_Text( pDX, IDC_LISTDESC, desc );
	DDX_Text( pDX, IDC_LISTID, id );
}

// this is called when the OK button is clicked
void CAddListItemDlg::OnOK() {
	UpdateData(); // use DDX to update the private data members
	// make sure the new id is positive
	if ( id <= 0 ) {
		MessageBox( "IDs must be greater than 0.", "" );
		return; // return control to the user
	}
	// make sure the string added to the list is not blank
	if ( desc == "" ) {
		MessageBox( "The description must not be blank.", "" );
		return; // return control to the user
	}
	// loop through the list to make sure the new id and string are not already in use
	list->Rewind();
	while ( list->HasMoreStrings() ) {
		string str = list->GetNextString();
		// see if they are in use
		if ( id == (*list)[str.c_str()] ) {
			MessageBox( "This id is already in use.", "" );
			return; // return control to the user
		}
		if ( desc == str.c_str() ) {
			MessageBox( "This description already has an entry.", "" );
			return; // return control to the user
		}
	}
	// attempt to add the new item to the list in the database
	if ( storage.AddListItem( list->GetName(), id, (LPCTSTR)desc ) ) {
		// it worked
		list->AddString( id, (LPCTSTR)desc ); // add the string to the local copy of the list
		CDialog::OnOK(); // close this dialog
	}
}

// this is called whenever the dialog box is shown
// it is overloaded so that a list to be added to can be given to the dialog
int CAddListItemDlg::DoModal( CIndexedList* newList ) {
	list = newList; // copy it into a private data member of this object
	return CDialog::DoModal(); // show the dialog box
}
