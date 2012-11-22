
#ifndef TABBUTTON_H
#define TABBUTTON_H

#include <afxwin.h>
#include <vector>
using namespace std;

class CTabButton : public CStatic {
	public:
		CTabButton();
		void OnPaint();
		void OnLButtonDown( UINT nFlags, CPoint point );
		void SetupTabs( vector<CTabButton*> tabs, vector<CDialog*> pages );
	private:
		bool active;
		vector< CTabButton* > tabs;
		vector< CDialog* > pages;
		DECLARE_MESSAGE_MAP()
};

#endif
