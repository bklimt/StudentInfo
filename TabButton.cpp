
#include "TabButton.h"

CTabButton::CTabButton() {
	active = false;
}

void CTabButton::OnPaint() {
	RECT rect;
	PAINTSTRUCT ps;
	CBrush brush( GetSysColor( COLOR_BTNFACE ) );
	CString text;
	CFont* font = CFont::FromHandle( (HFONT)GetStockObject( DEFAULT_GUI_FONT ) );
	CFont boldFont;
	GetClientRect( &rect );
	GetWindowText( text );
	CDC* dc = BeginPaint( &ps );
	CFont* oldFont = dc->SelectObject( font );

	// draw the background
	dc->FillRect( &rect, &brush );

	// draw the text
	if ( active ) {
		LOGFONT lFont;
		font->GetLogFont( &lFont );
		lFont.lfWeight = FW_BOLD;
		boldFont.CreateFontIndirect( &lFont );
		dc->SelectObject( &boldFont );
	}
	dc->SetBkMode( TRANSPARENT );
	dc->DrawText( text, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	// draw the border
	dc->MoveTo( rect.left, rect.bottom-1 );
	dc->LineTo( rect.left, rect.top );
	//dc->MoveTo( rect.left, rect.top );
	dc->LineTo( rect.right, rect.top );
	//dc->MoveTo( rect.right, rect.top );
	dc->LineTo( rect.right, rect.bottom );
	rect.top++;
	rect.left++;
	rect.right++;
	dc->DrawEdge( &rect, EDGE_RAISED, BF_LEFT | BF_RIGHT | BF_TOP );
	if ( !active ) {
		dc->MoveTo( rect.right-1, rect.bottom-1 );
		dc->LineTo( rect.left-1, rect.bottom-1 );
	}

	dc->SelectObject( oldFont );
	EndPaint( &ps );
}

void CTabButton::OnLButtonDown( UINT nFlags, CPoint point ) {
	for ( int i=0; i<tabs.size(); i++ ) {
		if ( tabs[i] == this ) {
			tabs[i]->active = true;
			pages[i]->ShowWindow( SW_SHOW );
			tabs[i]->Invalidate();
		} else {
			tabs[i]->active = false;
			pages[i]->ShowWindow( SW_HIDE );
			tabs[i]->Invalidate();
		}
	}
	CStatic::OnLButtonDblClk( nFlags, point );
}

void CTabButton::SetupTabs( vector<CTabButton*> newtabs, vector<CDialog*> newpages ) {
	tabs = newtabs;
	pages = newpages;
	if ( tabs[0] == this ) {
		active = true;
		//ShowWindow( SW_SHOW );
	} else {
		active = false;
	}
}

BEGIN_MESSAGE_MAP( CTabButton, CStatic )
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
