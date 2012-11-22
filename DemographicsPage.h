
#ifndef DEMOGRAPHICSPAGE_H
#define DEMOGRAPHICSPAGE_H

#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"
#include "StringEdit.h"
#include "MultiLineEdit.h"
#include "DateEdit.h"
#include "PhoneEdit.h"
#include "IndexedListBox.h"

class CDemographicsPage : public CDialog {
	public:
		CDemographicsPage();
		void DoDataExchange( CDataExchange* pDX );
		BOOL PreTranslateMessage( MSG* pMsg );
		void OnOK();
		void OnCancel();
		bool OnTabPressed();
		bool OnShiftTabPressed();
		void OnBirthDateChanged();
		void CopyAddressUp();
		void CopyAddressDown();

		CButton copyUp;
		CButton copyDown;

		CDateEdit   birthDate;
		CStringEdit chronAge;
		CStringEdit address;
		CStringEdit city;
		CStringEdit state;
		CStringEdit zipCode;
		CStringEdit mailingAddress;
		CStringEdit mailingCity;
		CStringEdit mailingState;
		CStringEdit mailingZipCode;
		CPhoneEdit  homePhone;

		CDateEdit   statusDate;
		CIndexedListBox status;
		CIndexedListBox eligibility;
		CIndexedListBox gender;
		CIndexedListBox ethnicity;
		CIndexedListBox campus;
		CIndexedListBox grade;

		CMultiLineEdit demographicsComments;

	private:
		DECLARE_MESSAGE_MAP()
};

#endif
