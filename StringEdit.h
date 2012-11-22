
/*
 * StringEdit.h
 *
 * This class is an edit box that is designed to only accept
 *   valid database strings ( with no single quotes )
 *
 */

// this will keep the file from being included twice
#ifndef STRINGEDIT_H
#define STRINGEDIT_H

#include <afxwin.h>
#include <string>
using namespace std;

class CStringEdit : public CEdit {
	public:
		CStringEdit();
		void OnKillFocus( CWnd* pNewWnd );
		virtual BOOL PreTranslateMessage( MSG* pMsg );

		// called to force the value in the editbox to a valid
		//   database string ( with no single quotes )
		virtual void ValidateData();

		// returns true if the data in the editbox has been
		//   changed since the last call to SetString()
		virtual bool IsModified();

		// sets the editbox to be considered unmodified
		virtual void ResetModified();

		// returns true if all of the digits in a string are digits
		//   this is used by several subclasses
		bool IsAllDigits( string s );

		// sets the string in the editbox to a particular value
		//   also, resets the control to be considered unmodified
		void SetString( string str );

		// returns the string currently in the editbox
		string GetString();

		// returns what the string in the editbox was last set to
		//   by SetString() or ResetModified()
		string GetOldText();

		void SetReadOnly( BOOL bReadOnly=TRUE );
		BOOL OnChange();
	private:
		// what the string in the editbox was last set to
		//   by SetString() or ResetModified()
		string oldText;
		bool readonly;

		DECLARE_DYNAMIC( CStringEdit );
		DECLARE_MESSAGE_MAP()
};

#endif
