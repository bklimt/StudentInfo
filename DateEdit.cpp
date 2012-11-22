
#include <string>
#include "DateEdit.h"
using namespace std;

void CDateEdit::ValidateData() {
	SetWindowText( VerifyFormat().c_str() );
	CStringEdit::ValidateData();
}

void CDateEdit::SetDate( CDate date ) {
	SetString( date );
}

CDate CDateEdit::GetDate() {
	return (CDate)VerifyFormat();
	/*if ( GetString() == VerifyFormat() ) {
		string text = GetString();
		if ( text == "" ) {
			return CDate();
		} else {
			return CDate(
				((text[0]-'0')*10)+(text[1]-'0'),
				((text[3]-'0')*10)+(text[4]-'0'),
				((text[6]-'0')*1000)+((text[7]-'0')*100)+((text[8]-'0')*10)+(text[9]-'0')
			);
		}
	} else {
		return CDate();
	}*/
}

string CDateEdit::VerifyFormat() {
	string text = GetString();
	if ( IsAllDigits( text ) ) {
		if ( text.length() == 6 ) {
			text = text.substr(0,2) + "-" + text.substr(2,2) + "-" + text.substr(4,2);
			//if ( text[4] < '6' ) {
			//	text = text.substr(0,2) + "-" + text.substr(2,2) + "-20" + text.substr(4,2);
			//} else {
			//	text = text.substr(0,2) + "-" + text.substr(2,2) + "-19" + text.substr(4,2);
			//}
		} else {
			if ( text.length() == 8 ) {
				text = text.substr(0,2) + "-" + text.substr(2,2) + "-" + text.substr(4,4);
			}
		}
	}

	return (string)CDate(text);

	/*
	if ( text.length() ) {
		if ( text.length() != 10 ) {
			return "";
			// SetWindowText( text.c_str() );
		} else {
			for ( int i=0; i<text.size(); i++ ) {
				switch ( i ) {
					case 0 :
					case 1 :
					case 3 :
					case 4 :
					case 6 :
					case 7 :
					case 8 :
					case 9 :
						if ( !isdigit( text[i] ) ) {
							return "";
							//SetWindowText("");
						}
						break;
					case 2 :
					case 5 :
						if ( text[i] != '-' ) {
							return "";
							//SetWindowText("");
						}
						break;
				}
			}
		}	
	}
	return GetString();
	*/
}
