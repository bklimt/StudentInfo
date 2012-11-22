
/*
 * DateEdit.h
 *
 * This class is an edit box that is designed to only accept dates
 *
 */

// this will keep the file from being included twice
#ifndef DATEEDIT_H
#define DATEEDIT_H

#include "StringEdit.h" // it is derived from StringEdit
#include "Date.h"       // this is the data type that this control edits

class CDateEdit : public CStringEdit {
	public:
		// called to force the value in the editbox to a valid date
		virtual void ValidateData();

		// sets the date in the editbox to a particular value
		//   also, resets the control to be considered unmodified
		void SetDate( CDate date );

		// returns the date currently in the editbox
		CDate GetDate();

		// returns what the value in this editbox would be forced
		//   to in order to become a valid date
		string VerifyFormat();
};

#endif
