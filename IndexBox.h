
#ifndef INDEXBOX_H
#define INDEXBOX_H

#include "StringEdit.h"

#include <afxwin.h>
#include <string>
using namespace std;

class CIndexBox : public CStringEdit {
	public:
		//void OnKillFocus( CWnd* pNewWnd );
		//string GetText();
		//bool IsAllDigits( string s );
		//void SetString( string str );
		void SetIndex( int index );
	private:
		DECLARE_MESSAGE_MAP()
};

#endif
