
#include "GridControlHelper.h"

// DO NOT DELETE THIS (it allows derived classes to have destructors)
CGridControlHelper::~CGridControlHelper() {
}

// called by CGridControl when a user has asked to modify a cell
void CGridControlHelper::OnBeginEdit( CRect& area, CVariant& data, CPoint* point, CWnd* parent ) {
	// do nothing - this is just a base class
}

// called by CGridControl when the user is through modifying the current cell
void CGridControlHelper::OnEndEdit( CVariant& data ) {
	// do nothing - this is just a base class
}

// called by CGridControl to Paint this cell
void CGridControlHelper::OnPaint( CDC* dc, CRect& area, CVariant& data ) {
	// if the Variant-to-string type-casting operator will work for
	//   a data type, then this function will not need to be overridden
	//   for the handler of that data type
	dc->Rectangle( area );
	dc->SetBkMode( TRANSPARENT );
	dc->DrawText(
		(CString)( ( (string)data ).c_str() ), 
		CRect( area.left+2, area.top+2, area.right-2, area.bottom-2 ),
		DT_SINGLELINE | DT_NOPREFIX
	);
}

// called to get the width a cell would like to be to fit all of its data
int CGridControlHelper::GetPreferredWidth( CDC* dc, CVariant& data ) {
	// see how long the text will be
	CString str = (LPCTSTR)( ( (string) data ).c_str() );
	CSize size = dc->GetTextExtent( str );
	return size.cx + 10;
}

// must return true if the value passed to it is null
bool CGridControlHelper::IsNull( CVariant& data ) {
	// just see if the data is null
	//   with subclasses, this may involve determining if
	//   the data is valid
	return ( data.GetType() == "null" );
}

// must return a pointer to the control that is created to let the user
//   modify the data in this cell, or NULL if there is not one
CWnd* CGridControlHelper::GetControl() {
	return NULL;
}

// must return true if the current value in the cell is different from
//   the argument specified
bool CGridControlHelper::IsModifiedFrom( CVariant& data ) {
	// data cannot be modified by this base class
	return false;
}
