
/* 
 * DateEditHelper.h
 *
 * This class allows dates (a DateEdit control) to be used in a GridControl
 *
 */

// make sure it is not included twice
#ifndef DATEEDITHELPER_H
#define DATEEDITHELPER_H

#include "GridControlHelper.h" // this class is derived from GridControlHelper
#include "DateEdit.h"          // this is the control that this helper handles

class CDateEditHelper : public CGridControlHelper {
	public:
		// called by CGridControl when a user has asked to modify a date cell
		void OnBeginEdit( CRect& area, CVariant& data, CPoint* point, CWnd* parent );

		// called by CGridControl when the user is through modifying the current cell
		void OnEndEdit( CVariant& data );

		// returns true if the value passed to it is a null date
		virtual bool IsNull( CVariant& data );

		// returns a pointer to the CDateEdit control (edit)
		virtual CWnd* GetControl();

		// returns true if the current value in the cell is different from "data"
		bool IsModifiedFrom( CVariant& data );

	private:
		CDateEdit edit; // the control that is used for editing the date
};

#endif
