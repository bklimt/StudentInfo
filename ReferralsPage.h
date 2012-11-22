
#ifndef REFERRALSPAGE_H
#define REFERRALSPAGE_H

#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"
#include "DateEdit.h"
#include "PhoneEdit.h"
#include "MultiLineEdit.h"
#include "IndexedListBox.h"

class CReferralsPage : public CDialog {
	public:
		CReferralsPage();
		void DoDataExchange( CDataExchange* pDX );
		BOOL PreTranslateMessage( MSG* pMsg );
		void OnOK();
		void OnCancel();
		bool OnTabPressed();
		bool OnShiftTabPressed();
		void OnAddEvaluation();
		void OnRemoveEvaluation();
		void OnReferralDateChanged();
		void OnEvaluationCompletedDateChanged();

		CDateEdit referralDate;
		CIndexedListBox referringCampus;
		CDateEdit evaluationDueDate;
		CDateEdit ardDueDate;
		CDateEdit evaluationCompletedDate;
		CDateEdit ardCompletedDate;
		CIndexedListBox referralEligible;
		CIndexedListBox evaluationRequestList;
		CListBox evaluationsRequested;

		CButton addEvaluationButton;
		CButton removeEvaluationButton;

		CMultiLineEdit referralsComments;
	private:
		DECLARE_MESSAGE_MAP()
};

#endif
