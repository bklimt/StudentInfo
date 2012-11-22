
#include "StudentEditWnd.h"
#include "StorageController.h"
#include "MainWindow.h"
#include "resource.h"

extern CStorageController storage;

BOOL CStudentEditWnd::Create( int schoolYearID, int x, int y, CMDIFrameWnd* parent ) {
	CStudent student( schoolYearID );

	BOOL retval = CMDIChildWnd::Create(
		NULL,
		"New Student",
		WS_CHILD | WS_OVERLAPPEDWINDOW,
		CRect( x, y, 610+x, 445+y ),
		parent,
		NULL
	);

	if ( retval ) {
		retval = dialog.Create( IDD_STUDENTEDIT, this );
	}

	dialog.ShowStudent( student, schoolYearID );

	CRect client;
	GetClientRect( client );
	if ( retval ) {
		dialog.SetWindowPos( &wndTop, 0, 0, client.Width(), client.Height(), SWP_SHOWWINDOW );
	}

	return retval;
}

BOOL CStudentEditWnd::Create( string ssn, int schoolYearID, int x, int y, CMDIFrameWnd* parent ) {
	CStudent student;
	if ( !storage.GetStudent( student, ssn, schoolYearID ) ) {
		return FALSE;
	}

	BOOL retval = FALSE;
	try {
		retval = CMDIChildWnd::Create(
			NULL,
			ssn.c_str(),
			WS_CHILD | WS_OVERLAPPEDWINDOW,
			CRect( x, y, 610+x, 445+y ),
			parent,
			NULL
		);

		if ( retval ) {
			retval = dialog.Create( IDD_STUDENTEDIT, this );
		}

		dialog.ShowStudent( student, schoolYearID );

		CRect client;
		GetClientRect( client );
		if ( retval ) {
			dialog.SetWindowPos( &wndTop, 0, 0, client.Width(), client.Height(), SWP_SHOWWINDOW );
		}
	}
	catch (...) {}

	return retval;
}

void CStudentEditWnd::OnClose() {
	if ( dialog.AttemptClose() ) {
		CMDIChildWnd::OnClose();
	}
}

/*void CStudentEditWnd::OnPaint() {
	CMDIChildWnd::OnPaint();
	dialog.Invalidate();
}*/

void CStudentEditWnd::OnSetFocus( CWnd* pOldWnd ) {
	CMDIChildWnd::OnSetFocus( pOldWnd );
	dialog.SetFocus();
}

void CStudentEditWnd::OnSize( UINT nType, int cx, int cy ) {
	if ( dialog.GetSafeHwnd() ) {
		dialog.SetWindowPos( &wndTop, 0, 0, cx, cy, SWP_NOREDRAW );
	}
	CMDIChildWnd::OnSize( nType, cx, cy );
}

BOOL CStudentEditWnd::OnEraseBkgnd( CDC* pDC ) {
	return TRUE;
}

void CStudentEditWnd::SaveStudent() {
	dialog.SaveStudent();
}

bool CStudentEditWnd::AttemptClose() {
	if ( dialog.AttemptClose() ) {
		DestroyWindow();
		return true;
	}
	return false;
}

bool CStudentEditWnd::IsModified() {
	return dialog.IsModified();
}

IMPLEMENT_DYNAMIC( CStudentEditWnd, CMDIChildWnd )

BEGIN_MESSAGE_MAP( CStudentEditWnd, CMDIChildWnd )
	ON_WM_CLOSE()
	//ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
