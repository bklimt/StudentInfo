
#include <string>
#include "SSNEdit.h"
using namespace std;

void CSSNEdit::ValidateData() {
	string text = GetString();
	if ( IsAllDigits( text ) ) {
		if ( text.length() == 9 ) {
			text = text.substr(0,3) + "-" + text.substr(3,2) + "-" + text.substr(5,4);
			SetWindowText( text.c_str() );
		}
	}
	if ( text.length() > 11 ) {
		text = text.substr(0,11);
		SetWindowText( text.c_str() );
	}
	//while ( text.length() < 11 ) {
	//	text = (string)"-" + text;
	//	SetWindowText( text.c_str() );
	//}

	bool isSSN = true;

	CStringEdit::ValidateData();
}
