
#define DECLARE_CHOOSER

#include "StudentChooserWnd.h"
#include "MainWindow.h"
#include "Preferences.h"
#include "resource.h"

extern CPreferences preferences;

BOOL CStudentChooserWnd::Create( CMDIFrameWnd* parent ) {

	BOOL retval = CMDIChildWnd::Create(
		NULL,
		"Student Chooser",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		preferences.GetRect( "StudentChooserRect" ),
		parent,
		NULL
	);

	if ( retval ) {
		retval = dialog.Create( IDD_STUDENTCHOOSER, this );
	}

	CRect client;
	GetClientRect( client );
	if ( retval ) {
		dialog.SetWindowPos( &wndTop, 0, 0, client.Width(), client.Height(), SWP_SHOWWINDOW );
	}

	return retval;
}

void CStudentChooserWnd::OnClose() {
	ShowWindow( SW_HIDE );
}

/*void CStudentChooserWnd::OnPaint() {
	CMDIChildWnd::OnPaint();
	dialog.Invalidate(FALSE);
}*/

void CStudentChooserWnd::OnSetFocus( CWnd* pOldWnd ) {
	CMDIChildWnd::OnSetFocus( pOldWnd );
	dialog.SetFocus();
}

void CStudentChooserWnd::OnSize( UINT nType, int cx, int cy ) {
	if ( dialog.GetSafeHwnd() ) {
		dialog.SetWindowPos( &wndTop, 0, 0, cx, cy, SWP_NOREDRAW );
	}
	CMDIChildWnd::OnSize( nType, cx, cy );
}

BOOL CStudentChooserWnd::OnEraseBkgnd( CDC* pDC ) {
	return TRUE;
}

void CStudentChooserWnd::RefreshStudentList() {
	dialog.RefreshStudentList();
}

void CStudentChooserWnd::NewStudent() {
	dialog.OnNewStudentButtonClicked();
}

void CStudentChooserWnd::CreateStudent() {
	dialog.CreateStudent();
}

void CStudentChooserWnd::OpenStudent() {
	dialog.OnStudentListDoubleClicked();
}

void CStudentChooserWnd::DeleteStudent() {
	dialog.OnDeleteStudentButtonClicked();
}

int CStudentChooserWnd::GetSchoolYearID() {
	return dialog.GetSchoolYearID();
}

IMPLEMENT_DYNAMIC( CStudentChooserWnd, CMDIChildWnd )

BEGIN_MESSAGE_MAP( CStudentChooserWnd, CMDIChildWnd )
	ON_WM_CLOSE()
	//ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
