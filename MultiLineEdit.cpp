
#include "MultiLineEdit.h"

BOOL CMultiLineEdit::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( (char)(pMsg->wParam) == '\r' ) {
			//char temp[1000];
			//int s,e;
			//GetSel(s,e);
			//sprintf( temp, "text: \"%s\"\nstart: %d\nend: %d\n", GetString().c_str(), s, e );
			//string t = temp;
			//for ( int i=0; i<GetString().length(); i++ ) {
			//	sprintf( temp, " %d", GetString()[i] );
			//	t += temp;
			//}
			//MessageBox( t.c_str() );
			if ( GetString() == "" ) {
				PostMessage( pMsg->message, (WPARAM)'\t', pMsg->lParam );
				return TRUE;
			} else {
				int start, end;
				GetSel( start, end );
				if ( start == end ) {
					string text = GetString();
					if ( end==text.length() && text[ text.length()-1 ]=='\n' ) {
						text = text.substr( 0, text.length()-1 );
						PostMessage( pMsg->message, (WPARAM)'\t', pMsg->lParam );
						return TRUE;
					}
				}
			}
		}
	}
	return CEdit::PreTranslateMessage( pMsg );
}
