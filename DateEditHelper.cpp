
/* 
 * DateEditHelper.cpp
 *
 * This class allows dates (a DateEdit control) to be used in a GridControl
 *
 */

#include "DateEditHelper.h"

// called by CGridControl when a user has asked to modify a date cell
void CDateEditHelper::OnBeginEdit( CRect& area, CVariant& data, CPoint* point, CWnd* parent ) {
	// create the CDateEdit editbox at the specified location
	edit.Create( WS_CHILD | WS_VISIBLE | WS_BORDER, area, parent, 2 );
	edit.SetFont( CFont::FromHandle( (HFONT)GetStockObject( DEFAULT_GUI_FONT ) ) );
	// give it the initial value specified
	edit.SetWindowText( ( (string)data ).c_str() );
	edit.SetFocus();
	// TODO: position the cursor where the user clicked
	if ( point ) {
		edit.PostMessage( WM_LBUTTONDOWN, NULL, ( point->y << 16 ) & point->x );
		edit.PostMessage( WM_LBUTTONUP, NULL, ( point->y << 16 ) & point->x );
	}
}

// called by CGridControl when the user is through modifying the current cell
void CDateEditHelper::OnEndEdit( CVariant& data ) {
	// remove the focus on the editbox
	//   this will force its data to be verified as a valid date
	edit.OnKillFocus( NULL );
	// get the new date for the cell
	data = edit.GetDate();
	// get rid of the editbox control
	edit.DestroyWindow();
}

// returns true if the value passed to it is a null date
bool CDateEditHelper::IsNull( CVariant& data ) {
	// pretty simple
	return ((CDate)data) == CDate();
}

// returns a pointer to the CDateEdit control (edit)
CWnd* CDateEditHelper::GetControl() {
	// even simpler
	return &edit;
}

// returns true if the current value in the cell is different from "data"
bool CDateEditHelper::IsModifiedFrom( CVariant& data ) {
	// is the date in the cell equivalent to the date specified?
	return !( edit.GetDate() == (CDate)data );
}

