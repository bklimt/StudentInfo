
/*
 * SSNEdit.h
 *
 * This class is an edit box that is designed to only accept
 *   Social Security Numbers
 *
 */

// this will keep the file from being included twice
#ifndef SSNEDIT_H
#define SSNEDIT_H

#include "StringEdit.h" // it is derived from StringEdit

class CSSNEdit : public CStringEdit {
	public:
		// called to force the value in the editbox to a valid SSN
		virtual void ValidateData();
};

#endif
