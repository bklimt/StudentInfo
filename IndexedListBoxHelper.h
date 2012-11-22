
/* 
 * IndexedListBoxHelper.h
 *
 * This class allows lists (an IndexedListBox) to be used in a GridControl
 *
 */

// make sure it is not included twice
#ifndef COMBOBOXHELPER_H
#define COMBOBOXHELPER_H

#include <afxwin.h>
#include "GridControlHelper.h" // this class is derived from GridControlHelper
#include "IndexedListBox.h"    // this is the control that this helper handles 

class CIndexedListBoxHelper : public CGridControlHelper {
	public:
		// constructor
		CIndexedListBoxHelper( CIndexedList newStringList );

		// called by CGridControl when a user has asked to medify a list cell
		virtual void OnBeginEdit( CRect& area, CVariant& data, CPoint* point, CWnd* parent );

		// called by CGridControl when the user is through modifying the current cell
		virtual void OnEndEdit( CVariant& data );

		// called by CGridControl to paint this cell
		virtual void OnPaint( CDC* dc, CRect& area, CVariant& data );

		// called to get the width the cell would like to be to fit all of its data
		virtual int  GetPreferredWidth( CDC* dc, CVariant& data );

		// returns true if the value passed to it is the index signifying that
		//   no item in the list is selected
		virtual bool IsNull( CVariant& data );

		// returns a pointer to the CIndexedListBox control (combo)
		virtual CWnd* GetControl();

		// returns true if the current value in the cell is different from "data"
		bool IsModifiedFrom( CVariant& data );

	private:
		CIndexedList stringList; // the list that this cell lets the user choose from
		CIndexedListBox combo; // the control that is used for editing the cell
};

#endif
