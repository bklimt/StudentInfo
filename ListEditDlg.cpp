
#include "ListEditDlg.h"
#include "AddListItemDlg.h"
#include "StorageController.h"

extern CStorageController storage;

CListEditDlg::CListEditDlg() : CDialog( IDD_LISTEDIT ) {
}

BOOL CListEditDlg::OnInitDialog() {
	BOOL retval = CDialog::OnInitDialog();
	listControl.InsertColumn( 0, "Description" );
	listControl.InsertColumn( 1, "ID" );
	listControl.CListCtrl::SetColumnWidth( 0, 150 );
	listControl.CListCtrl::SetColumnWidth( 1, 50 );
	list.Rewind();
	int i=0;
	while ( list.HasMoreStrings() ) {
		string str = list.GetNextString();
		listControl.InsertItem( i, str.c_str() );
		char temp[1000];
		sprintf( temp, "%d", list[ str.c_str() ] );
		listControl.SetItemText( i, 1, temp );
		i++;
	}
	return retval;
}

void CListEditDlg::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_LIST, listControl );
}

void CListEditDlg::OnAddListItemClicked() {
	CAddListItemDlg dialog;
	dialog.DoModal( &list );

	listControl.DeleteAllItems();
	list.Rewind();
	int i=0;
	while ( list.HasMoreStrings() ) {
		string str = list.GetNextString();
		listControl.InsertItem( i, str.c_str() );
		char temp[1000];
		sprintf( temp, "%d", list[ str.c_str() ] );
		listControl.SetItemText( i, 1, temp );
		i++;
	}
}

void CListEditDlg::OnRemoveListItemClicked() {
	int selected = listControl.GetSelectionMark();
	if ( selected != -1 ) {
		int index;
		sscanf( listControl.GetItemText( selected, 1 ), "%d", &index );
		if ( MessageBox( "Are you sure you want to delete this list entry?", "StudentInfo", MB_YESNO ) == IDYES ) {
			if ( storage.RemoveListItem( list.GetName(), index ) ) {
				listControl.DeleteItem( selected );
			}
		}
	}
}

void CListEditDlg::OnEndLabelEdit( NMHDR* pNMHDR, LRESULT* pResult ) {
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*) pNMHDR;
 	LV_ITEM	*plvItem = &plvDispInfo->item;

	if ( plvItem->pszText != NULL ) {
		int index;
		sscanf( listControl.GetItemText( plvItem->iItem, 1 ), "%d", &index );

		if ( storage.ModifyListItem( list.GetName(), index, plvItem->pszText ) ) {
			listControl.SetItemText( plvItem->iItem, plvItem->iSubItem, plvItem->pszText );
		}
	}
}

void CListEditDlg::EditList( CIndexedList newList ) {
	list = newList;
	DoModal();
}

BEGIN_MESSAGE_MAP( CListEditDlg, CDialog )
	ON_COMMAND( IDC_ADDLISTITEM, OnAddListItemClicked )
	ON_COMMAND( IDC_REMOVELISTITEM, OnRemoveListItemClicked )
	ON_NOTIFY( LVN_ENDLABELEDIT, IDC_LIST, OnEndLabelEdit )
END_MESSAGE_MAP()
