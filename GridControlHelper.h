
/* 
 * GridControlHelper.h
 *
 * This class is used as a base class for allowing a GridControl
 *   to handle a particular data type - see StringEditHelper, IndexedListBoxHelper
 *
 */

// make sure it is not included twice
#ifndef GRIDCONTROLHELPER_H
#define GRIDCONTROLHELPER_H

#include <afxwin.h>  // for CWnd, CRect, etc.
#include "Variant.h" // for CVariant

class CGridControlHelper {
	public:
		// DO NOT DELETE THIS (it allows derived classes to have destructors)
		virtual ~CGridControlHelper();

		// called by CGridControl when a user has asked to modify a cell
		virtual void OnBeginEdit( CRect& area, CVariant& data, CPoint* point, CWnd* parent );

		// called by CGridControl when the user is through modifying the current cell
		virtual void OnEndEdit( CVariant& data );

		// called by CGridControl to Paint this cell
		//   If the Variant-to-string type-casting operator will work for
		//   a data type, then this function will not need to be overridden
		//   for the handler of that data type.
		virtual void OnPaint( CDC* dc, CRect& area, CVariant& data );

		// called to get the width a cell would like to be to fit all of its data
		//   Override this if you override OnPaint().
		virtual int GetPreferredWidth( CDC* dc, CVariant& data );

		// must return true if the value passed to it is null
		//   With subclasses, this may involve determining if
		//   the data is valid.
		virtual bool IsNull( CVariant& data );

		// must return a pointer to the control that is created to let the user
		//   modify the data in this cell, or NULL if there is not one.
		virtual CWnd* GetControl();

		// must return true if the current value in the cell is different from
		//   the argument specified
		virtual bool IsModifiedFrom( CVariant& data );
};

#endif
