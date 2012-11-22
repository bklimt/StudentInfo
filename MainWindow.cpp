
//#define DECLARE_MAINWND

#include "MainWindow.h"
#include "Preferences.h"
#include "StudentChooserWnd.h"
#include "StudentEditWnd.h"
#include "ListEditDlg.h"
#include "UserListWnd.h"
#include "BackupDlg.h"
#include "RestoreDlg.h"
#include "StorageController.h"

extern CUser user;
extern CPreferences preferences;
extern CStorageController storage;

BOOL CMainWindow::Create() {
	newWindowX = newWindowY = 0;

	BOOL retval = LoadFrame( IDR_MAINWINDOW );

	userListWnd = NULL;
	
	if ( retval ) {
		retval = toolbar.Create( this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_FLYBY | CBRS_TOOLTIPS );
	}

	if ( retval ) {
		retval = toolbar.LoadToolBar( IDR_MAINWINDOW );
	}

	if ( retval ) {
		status.Create( this );
		UINT indicator = ID_SEPARATOR;
		status.SetIndicators( &indicator, 1 );
	}

	if ( retval ) {
		chooser = new CStudentChooserWnd();
		retval = chooser->Create( this );
	}

	chooser->ShowWindow( preferences.GetInt( "ChooserWindowState" ) );

	if ( !preferences.GetBool( "ShowStudentChooser" ) ) {
		chooser->ShowWindow( SW_HIDE );
	}

	return retval;
}

BOOL CMainWindow::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* context ) {
	return CreateClient( lpcs, NULL );
}

void CMainWindow::OnClose() {
	CWnd* child = GetWindow( GW_CHILD )->GetWindow( GW_CHILD );

	vector< CStudentEditWnd* > children;

	while ( child ) {
		if ( child->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) ) {
			children.push_back( (CStudentEditWnd*)child );
		}
		child = child->GetWindow( GW_HWNDNEXT );
	}

	for ( int i=0; i<children.size(); i++ ) {
		if ( !children[i]->AttemptClose() ) {
			return;
		}
	}

	if ( chooser->IsZoomed() ) {
		preferences.SetInt( "ChooserWindowState", SW_SHOWMAXIMIZED );
	} else {
		preferences.SetInt( "ChooserWindowState", SW_NORMAL );
	}
	chooser->ShowWindow( SW_NORMAL );

	if ( IsZoomed() ) {
		preferences.SetInt( "MainWindowState", SW_SHOWMAXIMIZED );
	} else {
		preferences.SetInt( "MainWindowState", SW_NORMAL );
	}
	ShowWindow( SW_NORMAL );

	CRect area;
	GetWindowRect( area );
	preferences.SetRect( "MainWindowRect", area );
	toolbar.ShowWindow( SW_HIDE );
	RecalcLayout();
	preferences.SetBool( "ShowStudentChooser", ( chooser->IsWindowVisible() ? true : false ) );
	CRect chooserArea;
	chooser->GetWindowRect( chooserArea );
	ScreenToClient( chooserArea );
	chooserArea.bottom-=2;
	chooserArea.left-=2;
	chooserArea.right-=2;
	chooserArea.top-=2;
	preferences.SetRect( "StudentChooserRect", chooserArea );
	preferences.SetInt( "CurrentSchoolYear", chooser->GetSchoolYearID() );

	CMDIFrameWnd::OnClose();
}

void CMainWindow::NewStudent( int schoolYearID ) {
	newWindowX = ( newWindowX + 10 ) % 100;
	newWindowY = ( newWindowY + 10 ) % 100;
	CStudentEditWnd* studentEditWnd = new CStudentEditWnd();
	studentEditWnd->Create( schoolYearID, newWindowX, newWindowY, this );
	studentEditWnd->ShowWindow( SW_SHOW );
	studentEditWnd->UpdateWindow();
	studentEditWnd->MDIActivate();
}

void CMainWindow::EditStudent( string ssn, int schoolYearID ) {
	newWindowX = ( newWindowX + 10 ) % 100;
	newWindowY = ( newWindowY + 10 ) % 100;
	CStudentEditWnd* studentEditWnd = new CStudentEditWnd();
	if ( studentEditWnd->Create( ssn, schoolYearID, newWindowX, newWindowY, this ) ) {
		studentEditWnd->ShowWindow( SW_SHOW );
		studentEditWnd->UpdateWindow();
		studentEditWnd->MDIActivate();
	} else {
		delete studentEditWnd;
	}
}

void CMainWindow::EditList( string listName ) {
	CListEditDlg dialog;
	dialog.EditList( storage.GetList( listName ) );
}

void CMainWindow::OnFileNew() {
	chooser->NewStudent();
}

void CMainWindow::OnFileOpen() {
	chooser->OpenStudent();
}

void CMainWindow::OnFileClose() {
	if ( MDIGetActive()->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) ) {
		((CStudentEditWnd*)MDIGetActive())->AttemptClose();
	}
}

void CMainWindow::OnFileShowOptionsOnNew() {
	preferences.SetBool( "ShowNewStudentOptions", !preferences.GetBool( "ShowNewStudentOptions" ) );
}

void CMainWindow::OnFileSave() {
	if ( MDIGetActive()->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) ) {
		((CStudentEditWnd*)MDIGetActive())->SaveStudent();
		//chooser->RefreshStudentList();
	}
}

void CMainWindow::OnFileSaveAll() {
	CWnd* child = GetWindow( GW_CHILD )->GetWindow( GW_CHILD );

	vector< CStudentEditWnd* > children;

	while ( child ) {
		if ( child->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) ) {
			children.push_back( (CStudentEditWnd*)child );
		}
		child = child->GetWindow( GW_HWNDNEXT );
	}

	for ( int i=0; i<children.size(); i++ ) {
		children[i]->SaveStudent();
	}
}

void CMainWindow::OnFileDelete() {
	chooser->DeleteStudent();
}

void CMainWindow::OnEditListDocument() { EditList( "Document" ); }
void CMainWindow::OnEditListDistributionMethod() { EditList( "DistributionMethod" ); }
void CMainWindow::OnEditListInitialDistribution() { EditList( "InitialDistribution" ); }

void CMainWindow::OnEditListCampus() { EditList( "Campus" ); }
void CMainWindow::OnEditListEligibility() { EditList( "Eligibility" ); }
void CMainWindow::OnEditListEthnicity() { EditList( "Ethnicity" ); }
void CMainWindow::OnEditListGender() { EditList( "Gender" ); }
void CMainWindow::OnEditListGrade() { EditList( "Grade" ); }
void CMainWindow::OnEditListStatus() { EditList( "Status" ); }
void CMainWindow::OnEditListAssessment() { EditList( "Assessment" ); }
void CMainWindow::OnEditListAssessmentMod() { EditList( "AssessmentMod" ); }
void CMainWindow::OnEditListAchLevel() { EditList( "AchLevel" ); }
void CMainWindow::OnEditListGradeLevel() { EditList( "GradeLevel" ); }
void CMainWindow::OnEditListDisability() { EditList( "Disability" ); }
void CMainWindow::OnEditListEvaluationEligible() { EditList( "EvaluationEligible" ); }
void CMainWindow::OnEditListReferralEligible() { EditList( "ReferralEligible" ); }
void CMainWindow::OnEditListEvaluationType() { EditList( "EvaluationType" ); }
void CMainWindow::OnEditListInstructionalSetting() { EditList( "InstructionalSetting" ); }
void CMainWindow::OnEditListESYServiceType() { EditList( "ESYServiceType" ); }
void CMainWindow::OnEditListInstructionalServiceType() { EditList( "InstructionalServiceType" ); }
void CMainWindow::OnEditListRelatedServiceType() { EditList( "RelatedServiceType" ); }
void CMainWindow::OnEditListServiceType() { EditList( "ServiceType" ); }
void CMainWindow::OnEditListSchoolYear() { EditList( "SchoolYear" ); }

void CMainWindow::OnAdminEditUsers() {
	if ( !userListWnd ) {
		userListWnd = new CUserListWnd();
		userListWnd->Create( this );
	}
	userListWnd->ShowWindow( SW_SHOW );
	userListWnd->SetFocus();
}

void CMainWindow::OnViewStudentChooser() {
	chooser->ShowWindow( SW_SHOW );
	chooser->SetFocus();
}

void CMainWindow::OnToolsBackup() {
	CBackupDlg backupDlg;
	backupDlg.DoModal();
}

void CMainWindow::OnToolsRestore() {
	CRestoreDlg restoreDlg;
	restoreDlg.DoModal();
	chooser->RefreshStudentList();
}

void CMainWindow::OnToolsQueryWizard() {
	string qwPath = preferences.GetHomePath() + "QueryBuilder.exe";
	if ( GetFileAttributes( qwPath.c_str() ) == -1 ) {
		MessageBox( "QueryBuilder is not installed.", "StudentInfo" );
	} else {
		if ( (int)ShellExecute( NULL, "open", qwPath.c_str(), NULL, preferences.GetHomePath().c_str(), SW_SHOWNORMAL ) <= 32 ) {
			MessageBox( "Unable to execute QueryBuilder.", "StudentInfo" );
		}
	}
}

void CMainWindow::OnWindowClose() {
	if ( MDIGetActive()->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) ) {
		((CStudentEditWnd*)MDIGetActive())->AttemptClose();
	}
}

void CMainWindow::OnWindowCloseAll() {
	CWnd* child = GetWindow( GW_CHILD )->GetWindow( GW_CHILD );

	vector< CStudentEditWnd* > children;

	while ( child ) {
		if ( child->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) ) {
			children.push_back( (CStudentEditWnd*)child );
		}
		child = child->GetWindow( GW_HWNDNEXT );
	}

	for ( int i=0; i<children.size(); i++ ) {
		if ( !children[i]->AttemptClose() ) {
			return;
		}
	}
}

void CMainWindow::OnWindowPrevious() {
	CWnd* prev = MDIGetActive()->GetWindow( GW_HWNDPREV );
	if ( prev ) {
		((CMDIChildWnd*)prev)->MDIActivate();
	}
}

void CMainWindow::OnWindowNext() {
	MDINext();
}

void CMainWindow::OnHelpAbout() {
	CDialog about( IDD_ABOUT );
	about.DoModal();
}

void CMainWindow::OnUpdateFileNew( CCmdUI* pCmdUI ) {
	pCmdUI->Enable( user.CanWrite() );
}

void CMainWindow::OnUpdateFileOpen( CCmdUI* pCmdUI ) {
	pCmdUI->Enable(
		MDIGetActive() &&
		MDIGetActive()->IsKindOf( RUNTIME_CLASS( CStudentChooserWnd ) ) &&
		chooser->IsWindowVisible()
	);
}

void CMainWindow::OnUpdateFileClose( CCmdUI* pCmdUI ) {
	pCmdUI->Enable( MDIGetActive() && MDIGetActive()->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) );
}

void CMainWindow::OnUpdateFileShowOptionsOnNew( CCmdUI* pCmdUI ) {
	pCmdUI->Enable( user.CanWrite() );
	pCmdUI->SetCheck( ( preferences.GetBool("ShowNewStudentOptions") ? 1 : 0 ) );
}

void CMainWindow::OnUpdateFileSave( CCmdUI* pCmdUI ) {
	pCmdUI->Enable(
		user.CanWrite() &&
		MDIGetActive() &&
		MDIGetActive()->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) &&
		((CStudentEditWnd*)MDIGetActive())->IsModified()
	);
}

void CMainWindow::OnUpdateFileDelete( CCmdUI* pCmdUI ) {
	pCmdUI->Enable(
		user.CanWrite() &&
		MDIGetActive() &&
		MDIGetActive()->IsKindOf( RUNTIME_CLASS( CStudentChooserWnd ) ) &&
		chooser->IsWindowVisible()
	);
}

void CMainWindow::OnUpdateViewStudentChooser( CCmdUI* pCmdUI ) {
	pCmdUI->SetCheck( chooser->IsWindowVisible() );
}

void CMainWindow::OnUpdateWindowClose( CCmdUI* pCmdUI ) {
	pCmdUI->Enable( MDIGetActive() && MDIGetActive()->IsKindOf( RUNTIME_CLASS( CStudentEditWnd ) ) );
}

void CMainWindow::OnUpdateEditListDocument                 ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListDistributionMethod       ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListInitialDistribution      ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListCampus                   ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListEligibility              ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListEthnicity                ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListGender                   ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListGrade                    ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListStatus                   ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListAssessment               ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListAssessmentMod            ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListAchLevel                 ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListGradeLevel               ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListDisability               ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListEvaluationEligible       ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListReferralEligible         ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListEvaluationType           ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListInstructionalSetting     ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListESYServiceType           ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListInstructionalServiceType ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListRelatedServiceType       ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListServiceType              ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }
void CMainWindow::OnUpdateEditListSchoolYear               ( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }

void CMainWindow::OnUpdateToolsQueryWizard( CCmdUI* pCmdUI ) {
	pCmdUI->Enable( GetFileAttributes( ( preferences.GetHomePath() + "QueryWizard.exe" ).c_str() ) != -1 );
}

void CMainWindow::OnUpdateToolsRestore( CCmdUI* pCmdUI ) {
	pCmdUI->Enable( user.IsAdmin() );
}

void CMainWindow::OnUpdateAdminEditUsers( CCmdUI* pCmdUI ) { pCmdUI->Enable( user.IsAdmin() ); }

BEGIN_MESSAGE_MAP( CMainWindow, CMDIFrameWnd )
	ON_WM_CLOSE()

	ON_COMMAND( ID_FILE_NEW, OnFileNew )
	ON_COMMAND( ID_FILE_OPEN, OnFileOpen )
	ON_COMMAND( ID_FILE_CLOSE, OnFileClose )
	ON_COMMAND( ID_FILE_SHOWOPTIONSONNEW, OnFileShowOptionsOnNew )
	ON_COMMAND( ID_FILE_SAVE, OnFileSave )
	ON_COMMAND( ID_FILE_SAVEALL, OnFileSaveAll )
	ON_COMMAND( ID_FILE_DELETE, OnFileDelete )

	ON_COMMAND( ID_EDITLIST_DOCUMENT, OnEditListDocument )
	ON_COMMAND( ID_EDITLIST_DISTRIBUTIONMETHOD, OnEditListDistributionMethod )
	ON_COMMAND( ID_EDITLIST_INITIALDISTRIBUTION, OnEditListInitialDistribution )

	ON_COMMAND( ID_EDITLIST_CAMPUS, OnEditListCampus )
	ON_COMMAND( ID_EDITLIST_ELIGIBILITY, OnEditListEligibility )
	ON_COMMAND( ID_EDITLIST_ETHNICITY, OnEditListEthnicity )
	ON_COMMAND( ID_EDITLIST_GENDER, OnEditListGender )
	ON_COMMAND( ID_EDITLIST_GRADE, OnEditListGrade )
	ON_COMMAND( ID_EDITLIST_STATUS, OnEditListStatus )
	ON_COMMAND( ID_EDITLIST_ASSESSMENT, OnEditListAssessment )
	ON_COMMAND( ID_EDITLIST_ASSESSMENTMOD, OnEditListAssessmentMod )
	ON_COMMAND( ID_EDITLIST_ACHLEVEL, OnEditListAchLevel )
	ON_COMMAND( ID_EDITLIST_GRADELEVEL, OnEditListGradeLevel )
	ON_COMMAND( ID_EDITLIST_DISABILITY, OnEditListDisability )
	ON_COMMAND( ID_EDITLIST_EVALUATIONELIGIBLE, OnEditListEvaluationEligible )
	ON_COMMAND( ID_EDITLIST_REFERRALELIGIBLE, OnEditListReferralEligible )
	ON_COMMAND( ID_EDITLIST_EVALUATIONTYPE, OnEditListEvaluationType )
	ON_COMMAND( ID_EDITLIST_INSTRUCTIONALSETTING, OnEditListInstructionalSetting )
	ON_COMMAND( ID_EDITLIST_ESYSERVICETYPE, OnEditListESYServiceType )
	ON_COMMAND( ID_EDITLIST_INSTRUCTIONALSERVICETYPE, OnEditListInstructionalServiceType )
	ON_COMMAND( ID_EDITLIST_RELATEDSERVICETYPE, OnEditListRelatedServiceType )
	ON_COMMAND( ID_EDITLIST_SERVICETYPE, OnEditListServiceType )
	ON_COMMAND( ID_EDITLIST_SCHOOLYEAR, OnEditListSchoolYear )
	ON_COMMAND( ID_ADMIN_EDITUSERS, OnAdminEditUsers )

	ON_COMMAND( ID_VIEW_STUDENTCHOOSER, OnViewStudentChooser )
	ON_COMMAND( ID_TOOLS_BACKUP, OnToolsBackup )
	ON_COMMAND( ID_TOOLS_RESTORE, OnToolsRestore )
	ON_COMMAND( ID_TOOLS_QUERY, OnToolsQueryWizard )

	ON_COMMAND( ID_WINDOW_CLOSE, OnWindowClose )
	ON_COMMAND( ID_WINDOW_CLOSEALL, OnWindowCloseAll )
	//ON_COMMAND( ID_WINDOW_PREVIOUS, OnWindowPrevious )
	ON_COMMAND( ID_WINDOW_NEXT, OnWindowNext )
	ON_COMMAND( ID_HELP_ABOUT, OnHelpAbout )

	ON_UPDATE_COMMAND_UI( ID_FILE_NEW, OnUpdateFileNew )
	ON_UPDATE_COMMAND_UI( ID_FILE_OPEN, OnUpdateFileOpen )
	ON_UPDATE_COMMAND_UI( ID_FILE_CLOSE, OnUpdateFileClose )
	ON_UPDATE_COMMAND_UI( ID_FILE_SHOWOPTIONSONNEW, OnUpdateFileShowOptionsOnNew )
	ON_UPDATE_COMMAND_UI( ID_FILE_SAVE, OnUpdateFileSave )
	ON_UPDATE_COMMAND_UI( ID_FILE_DELETE, OnUpdateFileDelete )
	ON_UPDATE_COMMAND_UI( ID_VIEW_STUDENTCHOOSER, OnUpdateViewStudentChooser )
	ON_UPDATE_COMMAND_UI( ID_WINDOW_CLOSE, OnUpdateWindowClose )

	ON_UPDATE_COMMAND_UI( ID_EDITLIST_DOCUMENT, OnUpdateEditListDocument )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_DISTRIBUTIONMETHOD, OnUpdateEditListDistributionMethod )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_INITIALDISTRIBUTION, OnUpdateEditListInitialDistribution )

	ON_UPDATE_COMMAND_UI( ID_EDITLIST_CAMPUS, OnUpdateEditListCampus )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_ELIGIBILITY, OnUpdateEditListEligibility )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_ETHNICITY, OnUpdateEditListEthnicity )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_GENDER, OnUpdateEditListGender )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_GRADE, OnUpdateEditListGrade )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_STATUS, OnUpdateEditListStatus )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_ASSESSMENT, OnUpdateEditListAssessment )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_ASSESSMENTMOD, OnUpdateEditListAssessmentMod )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_ACHLEVEL, OnUpdateEditListAchLevel )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_GRADELEVEL, OnUpdateEditListGradeLevel )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_DISABILITY, OnUpdateEditListDisability )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_EVALUATIONELIGIBLE, OnUpdateEditListEvaluationEligible )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_REFERRALELIGIBLE, OnUpdateEditListReferralEligible )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_EVALUATIONTYPE, OnUpdateEditListEvaluationType )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_INSTRUCTIONALSETTING, OnUpdateEditListInstructionalSetting )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_ESYSERVICETYPE, OnUpdateEditListESYServiceType )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_INSTRUCTIONALSERVICETYPE, OnUpdateEditListInstructionalServiceType )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_RELATEDSERVICETYPE, OnUpdateEditListRelatedServiceType )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_SERVICETYPE, OnUpdateEditListServiceType )
	ON_UPDATE_COMMAND_UI( ID_EDITLIST_SCHOOLYEAR, OnUpdateEditListSchoolYear )
	ON_UPDATE_COMMAND_UI( ID_TOOLS_QUERY, OnUpdateToolsQueryWizard )
	ON_UPDATE_COMMAND_UI( ID_TOOLS_RESTORE, OnUpdateToolsRestore )
	ON_UPDATE_COMMAND_UI( ID_ADMIN_EDITUSERS, OnUpdateAdminEditUsers )

END_MESSAGE_MAP()
