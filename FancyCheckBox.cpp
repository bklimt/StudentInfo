
#include "FancyCheckBox.h"

BOOL CFancyCheckBox::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN && (char)(pMsg->wParam) == '\r' ) {
		pMsg->wParam = '\t';
	}
	return CButton::PreTranslateMessage( pMsg );
}

bool CFancyCheckBox::GetValue() {
	return ( GetCheck() != 0 );
}

void CFancyCheckBox::SetValue( bool newValue ) {
	SetCheck( newValue ? 1 : 0 );
	oldValue = newValue;
}

bool CFancyCheckBox::IsModified() {
	return ( oldValue != GetValue() );
}

void CFancyCheckBox::ResetModified() {
	SetValue( GetValue() );
}

IMPLEMENT_DYNAMIC( CFancyCheckBox, CButton )
