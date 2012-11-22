
/*
 * StudentInfo.cpp
 *
 * This class is the application itself.  It is derived
 *   from CWinApp like any MFC application
 *
 */

#include "StudentInfo.h"
#include "MainWindow.h"  // main MDI window for the application
#include "LoginDlg.h"    // dialog to let users log in
#include "User.h"        // object that represents a user
#include "Preferences.h" // the app's set of preferences
#include "StorageController.h"

// instantiate the applications object
CStudentInfo app;

// declare all global objects
CStorageController storage;
CPreferences preferences;
CMainWindow* mainWnd;
CUser user;

// called when the application is run
//   InitInstance() is equivalent to main()
BOOL CStudentInfo::InitInstance() {
	string exePath = __argv[0];
	int filePos = exePath.find_last_of( '\\' );
	if ( filePos >= 0 ) {
		exePath = exePath.substr( 0, filePos+1 );
	}
	preferences.SetHomePath( exePath );
	preferences.LoadPreferences();

	// show the login dialog
	CLoginDlg login;
	login.DoModal();

	// create the main window
	mainWnd = new CMainWindow();
	mainWnd->Create();
	mainWnd->MoveWindow( preferences.GetRect( "MainWindowRect" ), FALSE );
	mainWnd->ShowWindow( preferences.GetInt( "MainWindowState" ) );
	mainWnd->UpdateWindow();

	// tell MFC it is the main window
	m_pMainWnd = mainWnd;

	return TRUE; // so the application will continue running
}
