
/* 
 * IndexedListBoxHelper.cpp
 *
 * This class allows lists (an IndexedListBox) to be used in a GridControl
 *
 */

#include "IndexedListBoxHelper.h"

// constructor
CIndexedListBoxHelper::CIndexedListBoxHelper( CIndexedList newStringList ) {
	stringList = newStringList;
}

// called by CGridControl when a user has asked to medify a list cell
void CIndexedListBoxHelper::OnBeginEdit( CRect& area, CVariant& data, CPoint* point, CWnd* parent ) {
	// create the combobox control
	combo.Create(
		WS_CHILD | WS_VISIBLE | WS_BORDER |WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect( area.left, area.top, area.right-1, area.bottom+200 ),
		parent, 2 );
	combo.SetFont( CFont::FromHandle( (HFONT)GetStockObject( DEFAULT_GUI_FONT ) ) );
	// give it the list to choose from
	combo.SetList( stringList );
	// select the item specified
	combo.Select( (int)data );
	combo.SetFocus();
}

// called by CGridControl when the user is through modifying the current cell
void CIndexedListBoxHelper::OnEndEdit( CVariant& data ) {
	// return the index of the selected item

	//CString text;
	//combo.GetWindowText( text );
	//data = stringList[ (char*)(LPCTSTR)text ];

	data = combo.GetSelectedIndex();
	combo.DestroyWindow();
}

// called by CGridControl to paint this cell
void CIndexedListBoxHelper::OnPaint( CDC* dc, CRect& area, CVariant& data ) {
	// draw the text that is selected, i.e. its index == data
	dc->Rectangle( area );
	dc->SetBkMode( TRANSPARENT );
	CString text = stringList[ (int)data ].c_str();
	dc->DrawText(
		text,
		CRect( area.left+2, area.top+2, area.right-2, area.bottom-2 ),
		DT_SINGLELINE | DT_NOPREFIX
	);
}

// called to get the width the cell would like to be to fit all of its data
int CIndexedListBoxHelper::GetPreferredWidth( CDC* dc, CVariant& data ) {
	int maxWidth = 0;
	CString text;
	CSize size;

	// look up the string that is selected ( this code should go away )
	// text = stringList[ (int)data ].c_str();
	// size = dc->GetTextExtent( text );
	// maxWidth = size.cx;

	// find the longest string in the list
	stringList.Rewind();
	while ( stringList.HasMoreStrings() ) {
		string item = stringList.GetNextString();
		size = dc->GetTextExtent( item.c_str() );
		maxWidth = ( size.cx > maxWidth ? size.cx : maxWidth );
	}

	// add room for the scrollbar in the combo box
	return maxWidth + 10;
}

// returns true if the value passed to it is the index signifying that
//   no item in the list is selected
bool CIndexedListBoxHelper::IsNull( CVariant& data ) {
	return ((int)data == -1);
}

// returns a pointer to the CIndexedListBox control (combo)
CWnd* CIndexedListBoxHelper::GetControl() {
	return &combo;
}

// returns true if the current value in the cell is different from "data"
bool CIndexedListBoxHelper::IsModifiedFrom( CVariant& data ) {
	return ( combo.GetSelectedIndex() != (int)data );
}
