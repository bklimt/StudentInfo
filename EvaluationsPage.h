
#ifndef EVALUATIONSPAGE_H
#define EVALUATIONSPAGE_H

#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"
#include "PhoneEdit.h"
#include "MultiLineEdit.h"
#include "IndexBox.h"
#include "FancyCheckBox.h"
#include "IndexedListBox.h"
#include "GridControl.h"

class CEvaluationsPage : public CDialog {
	public:
		CEvaluationsPage();
		void DoDataExchange( CDataExchange* pDX );
		BOOL PreTranslateMessage( MSG* pMsg );
		void OnOK();
		void OnCancel();
		bool OnTabPressed();
		bool OnShiftTabPressed();
		void UpdateDisabilityCodes();

		CIndexedListBox primaryDisability;
		CIndexedListBox secondaryDisability;
		CIndexedListBox tertiaryDisability;
		CIndexBox primaryDisabilityCode;
		CIndexBox secondaryDisabilityCode;
		CIndexBox tertiaryDisabilityCode;
		CGridControl evaluations;
		CMultiLineEdit evaluationsComments;
		CFancyCheckBox multipleDisabilities;
	private:
		DECLARE_MESSAGE_MAP()
};

#endif
