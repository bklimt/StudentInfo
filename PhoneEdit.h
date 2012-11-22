
#ifndef PHONEEDIT_H
#define PHONEEDIT_H

#include <afxwin.h>
#include "StringEdit.h"

class CPhoneEdit : public CStringEdit {
	public:
		virtual void ValidateData();
};

#endif
