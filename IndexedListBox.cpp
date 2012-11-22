
#include "IndexedListBox.h"

CIndexedListBox::CIndexedListBox() {
	typed = "";
	allowNull = true;
	oldIndex = -1;
	readonly = false;
}

BOOL CIndexedListBox::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( (int)pMsg->wParam == VK_RETURN ) {
			PostMessage( WM_KEYDOWN, VK_TAB );
		}	
	}
	return CComboBox::PreTranslateMessage( pMsg );
}

void CIndexedListBox::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags ) {
	if ( nChar == '\b' ) {
		SetCurSel( 0 );
		typed = "";
	} else {
		if ( FindString( -1, ( typed+(char)nChar ).c_str() ) > 0 ) {
			typed += (char)nChar;
		}
	}
	SelectString( -1, typed.c_str() );
}

void CIndexedListBox::OnKillFocus( CWnd* pNewWnd ) {
	CComboBox::OnKillFocus( pNewWnd );
	typed = "";
}

void CIndexedListBox::SetReadOnly( BOOL bReadOnly ) {
	if ( bReadOnly && !readonly ) {
		readonly = true;
		backupList = stringList;
		backupAllowNull = allowNull;
		CIndexedList newList;
		if ( GetSelectedIndex() == -1 ) {
			EnableNull();
		} else {
			newList.AddString( GetSelectedIndex(), stringList[GetSelectedIndex()] );
			DisableNull();
		}
		SetList( newList );
	} else {
		if ( !bReadOnly && readonly ) {
			readonly = false;
			stringList = backupList;
			allowNull = backupAllowNull;
			SetList( backupList );
		}
	}
}

void CIndexedListBox::DisableNull() {
	allowNull = false;
}

void CIndexedListBox::EnableNull() {
	allowNull = true;
}

void CIndexedListBox::SetList( CIndexedList newlist ) {
	stringList = newlist;
	ResetContent();
	if ( allowNull ) {
		AddString("");
	}
	stringList.Rewind();
	while ( stringList.HasMoreStrings() ) {
		AddString( stringList.GetNextString().c_str() );
	}
	if ( !allowNull ) {
		SetCurSel( 0 );
	}
}

void CIndexedListBox::Select( int key ) {
	if ( readonly ) {
		oldIndex = key;
		CIndexedList newList;
		if ( key == -1 ) {
			EnableNull();
		} else {
			newList.AddString( key, backupList[key] );
			DisableNull();
		}
		SetList( newList );
	} else {
		oldIndex = key;
		if ( key < 0 && allowNull ) {
			SetCurSel( 0 );
		} else {
			SelectString( -1, stringList[key].c_str() );
		}
	}
}

int CIndexedListBox::GetSelectedIndex() {
	CString text;
	if ( GetCurSel() < 0 ) {
		return -1;
	} else {
		GetLBText( GetCurSel(), text );
		return stringList[(char*)(LPCTSTR)text];
	}
}

bool CIndexedListBox::IsModified() {
	return ( GetSelectedIndex() != oldIndex );
}

void CIndexedListBox::ResetModified() {
	Select( GetSelectedIndex() );
}

CIndexedList* CIndexedListBox::GetList() {
	return &stringList;
}

IMPLEMENT_DYNAMIC( CIndexedListBox, CComboBox )

BEGIN_MESSAGE_MAP( CIndexedListBox, CComboBox )
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
