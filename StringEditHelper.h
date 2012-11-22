
/* 
 * StringEditHelper.h
 *
 * This class allows strings (a StringEdit control) to be used in a GridControl
 *
 */

// make sure it is not included twice
#ifndef STRINGEDITHELPER_H
#define STRINGEDITHELPER_H

#include "GridControlHelper.h" // this class is derived from GridControlHelper
#include "StringEdit.h"        // this is the control that this helper handles

class CStringEditHelper : public CGridControlHelper {
	public:
		// called by CGridControl when a user has asked to medify a string cell
		void OnBeginEdit( CRect& area, CVariant& data, CPoint* point, CWnd* parent );
		// called by CGridControl when the user is through modifying the current cell
		void OnEndEdit( CVariant& data );
		// returns true if the value passed to it is ""
		virtual bool IsNull( CVariant& data );
		// returns a pointer to the CStringEdit control (edit)
		virtual CWnd* GetControl();
		// returns true if the current value in the cell is different from "data"
		bool IsModifiedFrom( CVariant& data );
	private:
		CStringEdit edit; // the control that is used for editing the string
};

#endif
