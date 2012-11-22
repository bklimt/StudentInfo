
#ifndef CONTACTSPAGE_H
#define CONTACTSPAGE_H

#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"
#include "PhoneEdit.h"
#include "MultiLineEdit.h"

class CContactsPage : public CDialog {
	public:
		CContactsPage();
		void DoDataExchange( CDataExchange* pDX );
		BOOL PreTranslateMessage( MSG* pMsg );
		void OnOK();
		void OnCancel();
		bool OnTabPressed();
		bool OnShiftTabPressed();

		CStringEdit motherFirstName;
		CStringEdit motherLastName;
		CStringEdit motherAddress;
		CStringEdit motherCity;
		CStringEdit motherState;
		CStringEdit motherZipCode;
		CPhoneEdit  motherWorkPhone;
		CPhoneEdit  motherHomePhone;
		CPhoneEdit  motherCellPhone;
		CStringEdit motherEmail;

		CStringEdit fatherFirstName;
		CStringEdit fatherLastName;
		CStringEdit fatherAddress;
		CStringEdit fatherCity;
		CStringEdit fatherState;
		CStringEdit fatherZipCode;
		CPhoneEdit  fatherWorkPhone;
		CPhoneEdit  fatherHomePhone;
		CPhoneEdit  fatherCellPhone;
		CStringEdit fatherEmail;

		CMultiLineEdit contactsComments;
};

#endif
