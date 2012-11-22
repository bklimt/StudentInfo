
#include <string>
#include "PhoneEdit.h"
using namespace std;

void CPhoneEdit::ValidateData() {
	string number = GetString();
	
	if ( IsAllDigits( number ) ) {
		if ( number.length() == 10 ) {
			number = number.substr(0,3) + "-" + number.substr(3,3) + "-" + number.substr(6,4);
		} else {
			if ( number.length() == 7 ) {
				number = (string)"254-" + number.substr(0,3) + "-" + number.substr(3,4);
			}
		}
		SetWindowText( number.c_str() );
	}

	CStringEdit::ValidateData();
}
