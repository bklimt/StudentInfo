
#include "StringEdit.h"

CStringEdit::CStringEdit() : CEdit() {
	readonly = false;
}

void CStringEdit::OnKillFocus( CWnd* pNewWnd ) {
	CEdit::OnKillFocus( pNewWnd );
	ValidateData();
}

void CStringEdit::ValidateData() {
	string text = GetString();
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

BOOL CStringEdit::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( (char)(pMsg->wParam) == '\r' ) {
			//pMsg->wParam = '\t';
			PostMessage( pMsg->message, (WPARAM)'\t', pMsg->lParam );
		}
	}
	return CEdit::PreTranslateMessage( pMsg );
}

string CStringEdit::GetString() {
	CString str;
	GetWindowText( str );
	string s = (LPCTSTR)str;

	while ( s.length() && isspace( s[0] ) ) {
		s = s.substr( 1, s.length()-1 );
	}

	while ( s.length() && isspace( s[ s.length()-1 ] ) ) {
		s = s.substr( 0, s.length()-1 );
	}

	return s;
}

string CStringEdit::GetOldText() {
	return oldText;
}

bool CStringEdit::IsModified() {
	return ( GetString() != oldText );
}

void CStringEdit::ResetModified() {
	SetString( GetString() );
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
	oldText = str;
	SetWindowText( str.c_str() );
}

void CStringEdit::SetReadOnly( BOOL bReadOnly ) {
	readonly = bReadOnly ? true : false;
}

BOOL CStringEdit::OnChange() {
	if ( readonly && IsModified() ) {
		SetString( GetOldText() );
	}
	return FALSE;
}

IMPLEMENT_DYNAMIC( CStringEdit, CEdit )

BEGIN_MESSAGE_MAP( CStringEdit, CEdit )
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT_EX( EN_CHANGE, OnChange )
END_MESSAGE_MAP()

