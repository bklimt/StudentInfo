
#ifndef MULTILINEEDIT_H
#define MULTILINEEDIT_H

#include <afxwin.h>
#include <string>
#include "StringEdit.h"
using namespace std;

class CMultiLineEdit : public CStringEdit {
	public:
		virtual BOOL PreTranslateMessage( MSG* pMsg );
};

#endif
