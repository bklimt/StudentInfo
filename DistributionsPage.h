
#ifndef DISTRIBUTIONSPAGE_H
#define DISTRIBUTIONSPAGE_H

#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"
#include "GridControl.h"
#include "MultiLineEdit.h"

class CDistributionsPage : public CDialog {
	public:
		CDistributionsPage();
		void DoDataExchange( CDataExchange* pDX );
		BOOL PreTranslateMessage( MSG* pMsg );
		void OnOK();
		void OnCancel();
		bool OnTabPressed();
		bool OnShiftTabPressed();

		CGridControl distributions;
		CMultiLineEdit distributionsComments;
	private:
		DECLARE_MESSAGE_MAP()
};

#endif
