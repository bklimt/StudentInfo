
#ifndef SERVICESPAGE_H
#define SERVICESPAGE_H

#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"
#include "DateEdit.h"
#include "IndexBox.h"
#include "FancyCheckBox.h"
#include "MultiLineEdit.h"
#include "IndexedListBox.h"
#include "GridControl.h"

class CServicesPage : public CDialog {
	public:
		CServicesPage();
		void DoDataExchange( CDataExchange* pDX );
		BOOL PreTranslateMessage( MSG* pMsg );
		void OnOK();
		void OnCancel();
		bool OnTabPressed();
		bool OnShiftTabPressed();
		void UpdateInstructionalSettingCode();

		CIndexedListBox instructionalSetting;
		CIndexBox instructionalSettingCode;
		CDateEdit lastARD;
		CDateEdit lastAnnualReview;
		CDateEdit itpDate;
		CDateEdit iepStartDate;
		CDateEdit bipDate;
		CDateEdit iepEndDate;
		CGridControl instructionalServices;
		CGridControl relatedServices;
		CGridControl esyServices;
		CMultiLineEdit servicesComments;
		CFancyCheckBox medicallyFragile;
		CFancyCheckBox medicaid;

	private:
		DECLARE_MESSAGE_MAP()
};

#endif
