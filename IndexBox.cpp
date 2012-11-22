
#include "IndexBox.h"

void CIndexBox::SetIndex( int index ) {
	if ( index == -1 ) {
		SetString( "" );
	} else {
		char temp[1000];
		sprintf( temp, "%d", index );
		SetString( temp );
	}
}

/*void CStringEdit::OnKillFocus( CWnd* pNewWnd ) {
	CEdit::OnKillFocus( pNewWnd );
	string text = GetText();
	bool quoteFound = false;
	for ( int i=0; i<text.length(); i++ ) {
		if ( text[i]=='\'' ) {
			quoteFound = true;
			text[i] = '`';
		}
	}
	if ( quoteFound ) {
		SetWindowText( text.c_str() );
	}
}

string CStringEdit::GetText() {
	CString str;
	GetWindowText( str );
	return (LPCTSTR)str;
}

bool CStringEdit::IsAllDigits( string s ) {
	for ( int i=0; i<s.length(); i++ ) {
		if ( s[i]<'0' || s[i]>'9' ) {
			return false;
		}
	}
	return true;
}

void CStringEdit::SetString( string str ) {
	SetWindowText( str.c_str() );
}
*/

BEGIN_MESSAGE_MAP( CIndexBox, CEdit )
//	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()