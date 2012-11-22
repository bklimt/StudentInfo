
#include <afxwin.h>
#include <afxcmn.h>
#include "IndexedList.h"
#include "resource.h"

class CListEditDlg : public CDialog {
	public:
		CListEditDlg();
		BOOL OnInitDialog();
		void DoDataExchange( CDataExchange* pDX );
		void OnAddListItemClicked();
		void OnRemoveListItemClicked();
		void OnEndLabelEdit( NMHDR* pNMHDR, LRESULT* pResult );
		void EditList( CIndexedList newList );

	private:
		CListCtrl listControl;
		CIndexedList list;
		DECLARE_MESSAGE_MAP()
};
