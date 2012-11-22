
#include "CheckIndexedListBox.h"

BOOL CCheckIndexedListBox::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( (int)pMsg->wParam == VK_RETURN ) {
			PostMessage( WM_KEYDOWN, VK_TAB );
		}	
	}
	return CCheckListBox::PreTranslateMessage( pMsg );
}

void CCheckIndexedListBox::SetReadOnly( BOOL bReadOnly ) {
	for ( int i=0; i<GetCount(); i++ ) {
		Enable( i, !bReadOnly );
	}
}

void CCheckIndexedListBox::SetList( CIndexedList newlist ) {
	stringList = newlist;
	ResetContent();
	
	stringList.Rewind();
	while ( stringList.HasMoreStrings() ) {
		AddString( stringList.GetNextString().c_str() );
	}
}

void CCheckIndexedListBox::Select( vector<int>& key ) {
	for ( int i=0; i<GetCount(); i++ ) {
		SetCheck( i, 0 );
	}
	for ( i=0; i<key.size(); i++ ) {
		SetCheck( FindStringExact( -1, stringList[key[i]].c_str() ), 1 );
	}
}

vector<int> CCheckIndexedListBox::GetSelectedIndices() {
	vector<int> ans;
	for ( int i=0; i<GetCount(); i++ ) {
		if ( GetCheck( i ) ) {
			CString text;
			GetText( i, text );
			ans.push_back( stringList[(LPCTSTR)text] );
		}
	}
	return ans;
}

bool CCheckIndexedListBox::IsModified() {
	vector<int> newIndices = GetSelectedIndices();
	if ( newIndices.size() != oldIndices.size() ) {
		return true;
	}
	for ( int i=0; i<newIndices.size(); i++ ) {
		if ( newIndices[i] != oldIndices[i] ) {
			return true;
		}
	}
	return false;
}

void CCheckIndexedListBox::ResetModified() {
	oldIndices = GetSelectedIndices();
}

CIndexedList* CCheckIndexedListBox::GetList() {
	return &stringList;
}

IMPLEMENT_DYNAMIC( CCheckIndexedListBox, CCheckListBox )
