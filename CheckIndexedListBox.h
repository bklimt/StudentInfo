
#ifndef CHECKINDEXEDLISTBOX_H
#define CHECKINDEXEDLISTBOX_H

#include <vector>
#include <afxwin.h>
#include "IndexedList.h"
using namespace std;

class CCheckIndexedListBox : public CCheckListBox {
	public:
		BOOL PreTranslateMessage( MSG* pMsg );
		void SetReadOnly( BOOL bReadOnly = TRUE );
		void SetList( CIndexedList newlist );
		void Select( vector<int>& key );
		vector<int> GetSelectedIndices();
		bool IsModified();
		void ResetModified();
		CIndexedList* GetList();
	private:
		vector<int> oldIndices;
		CIndexedList stringList;

		DECLARE_DYNAMIC( CCheckIndexedListBox )
};

#endif
