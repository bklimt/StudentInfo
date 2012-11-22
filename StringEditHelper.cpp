
/* 
 * StringEditHelper.cpp
 *
 * This class allows strings (a StringEdit control) to be used in a GridControl
 *
 */

#include "StringEditHelper.h"

// called by CGridControl when a user has asked to medify a string cell
void CStringEditHelper::OnBeginEdit( CRect& area, CVariant& data, CPoint* point, CWnd* parent ) {
	// create the edit control
	edit.Create( WS_CHILD | WS_VISIBLE | WS_BORDER, area, parent, 2 );
	edit.SetFont( CFont::FromHandle( (HFONT)GetStockObject( DEFAULT_GUI_FONT ) ) );
	// give it the string to display
	edit.SetString( data );
	edit.SetFocus();

	// TODO: position the cursor where the user clicked
	//   this might work
	if ( point ) {
		point->x -= area.left;
		point->y -= area.top;
		edit.PostMessage( WM_LBUTTONDOWN, MK_LBUTTON, ( point->y << 16 ) & point->x );
		edit.PostMessage( WM_LBUTTONUP, MK_LBUTTON, ( point->y << 16 ) & point->x );
	}
}

// called by CGridControl when the user is through modifying the current cell
void CStringEditHelper::OnEndEdit( CVariant& data ) {
	// make sure this string is valid
	edit.ValidateData();
	// get the new value for the cell
	data = edit.GetString();
	edit.DestroyWindow();
}

// returns true if the value passed to it is ""
bool CStringEditHelper::IsNull( CVariant& data ) {
	return ((string)data=="");
}

// returns a pointer to the CStringEdit control (edit)
CWnd* CStringEditHelper::GetControl() {
	return &edit;
}

// returns true if the current value in the cell is different from "data"
bool CStringEditHelper::IsModifiedFrom( CVariant& data ) {
	return edit.GetString() != (string)data;
}
