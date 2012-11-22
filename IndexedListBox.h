
#ifndef INDEXEDLISTBOX_H
#define INDEXEDLISTBOX_H

#include <afxwin.h>
#include <string>
#include "IndexedList.h"
using namespace std;

class CIndexedListBox : public CComboBox {
	public:
		CIndexedListBox();
		BOOL PreTranslateMessage( MSG* pMsg );
		void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
		void OnKillFocus( CWnd* pNewWnd );
		void SetReadOnly( BOOL bReadOnly = TRUE );
		void DisableNull();
		void EnableNull();
		void SetList( CIndexedList newlist );
		void Select( int key );
		int GetSelectedIndex();
		bool IsModified();
		void ResetModified();
		CIndexedList* GetList();
	private:
		int oldIndex;
		bool allowNull;
		string typed;
		CIndexedList stringList;

		CIndexedList backupList;
		bool backupAllowNull;
		bool readonly;

		DECLARE_DYNAMIC( CIndexedListBox )
		DECLARE_MESSAGE_MAP()
};

#endif
