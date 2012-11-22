
#include <afxwin.h>
#include <afxext.h>
#include <string>
using namespace std;

class CMainWindow : public CMDIFrameWnd {
	public:
		BOOL Create();
		BOOL OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* context );
		void OnClose();

		void NewStudent( int schoolYearID );
		void EditStudent( string ssn, int schoolYearID );
		void EditList( string listName );

		void OnFileNew();
		void OnFileOpen();
		void OnFileClose();
		void OnFileShowOptionsOnNew();
		void OnFileSave();
		void OnFileSaveAll();
		void OnFileDelete();

		void OnEditListDocument();
		void OnEditListDistributionMethod();
		void OnEditListInitialDistribution();

		void OnEditListCampus();
		void OnEditListEligibility();
		void OnEditListEthnicity();
		void OnEditListGender();
		void OnEditListGrade();
		void OnEditListStatus();
		void OnEditListAssessment();
		void OnEditListAssessmentMod();
		void OnEditListAchLevel();
		void OnEditListGradeLevel();
		void OnEditListDisability();
		void OnEditListEvaluationEligible();
		void OnEditListReferralEligible();
		void OnEditListEvaluationType();
		void OnEditListInstructionalSetting();
		void OnEditListESYServiceType();
		void OnEditListInstructionalServiceType();
		void OnEditListRelatedServiceType();
		void OnEditListServiceType();
		void OnEditListSchoolYear();
		void OnAdminEditUsers();

		void OnViewStudentChooser();
		void OnToolsBackup();
		void OnToolsRestore();
		void OnToolsQueryWizard();

		void OnWindowClose();
		void OnWindowCloseAll();
		void OnWindowPrevious();
		void OnWindowNext();

		void OnHelpAbout();

		void OnUpdateFileNew( CCmdUI* pCmdUI );
		void OnUpdateFileOpen( CCmdUI* pCmdUI );
		void OnUpdateFileClose( CCmdUI* pCmdUI );
		void OnUpdateFileShowOptionsOnNew( CCmdUI* pCmdUI );
		void OnUpdateFileSave( CCmdUI* pCmdUI );
		void OnUpdateFileDelete( CCmdUI* pCmdUI );
		void OnUpdateViewStudentChooser( CCmdUI* pCmdUI );
		void OnUpdateWindowClose( CCmdUI* pCmdUI );

		void OnUpdateEditListDocument( CCmdUI* pCmdUI );
		void OnUpdateEditListDistributionMethod( CCmdUI* pCmdUI );
		void OnUpdateEditListInitialDistribution( CCmdUI* pCmdUI );

		void OnUpdateEditListCampus( CCmdUI* pCmdUI );
		void OnUpdateEditListEligibility( CCmdUI* pCmdUI );
		void OnUpdateEditListEthnicity( CCmdUI* pCmdUI );
		void OnUpdateEditListGender( CCmdUI* pCmdUI );
		void OnUpdateEditListGrade( CCmdUI* pCmdUI );
		void OnUpdateEditListStatus( CCmdUI* pCmdUI );
		void OnUpdateEditListAssessment( CCmdUI* pCmdUI );
		void OnUpdateEditListAssessmentMod( CCmdUI* pCmdUI );
		void OnUpdateEditListAchLevel( CCmdUI* pCmdUI );
		void OnUpdateEditListGradeLevel( CCmdUI* pCmdUI );
		void OnUpdateEditListDisability( CCmdUI* pCmdUI );
		void OnUpdateEditListEvaluationEligible( CCmdUI* pCmdUI );
		void OnUpdateEditListReferralEligible( CCmdUI* pCmdUI );
		void OnUpdateEditListEvaluationType( CCmdUI* pCmdUI );
		void OnUpdateEditListInstructionalSetting( CCmdUI* pCmdUI );
		void OnUpdateEditListESYServiceType( CCmdUI* pCmdUI );
		void OnUpdateEditListInstructionalServiceType( CCmdUI* pCmdUI );
		void OnUpdateEditListRelatedServiceType( CCmdUI* pCmdUI );
		void OnUpdateEditListServiceType( CCmdUI* pCmdUI );
		void OnUpdateEditListSchoolYear( CCmdUI* pCmdUI );

		void OnUpdateToolsQueryWizard( CCmdUI* pCmdUI );
		void OnUpdateToolsRestore( CCmdUI* pCmdUI );
		void OnUpdateAdminEditUsers( CCmdUI* pCmdUI );

	private:
		CToolBar toolbar;
		CStatusBar status;
		int newWindowX, newWindowY;
		DECLARE_MESSAGE_MAP()
};

//#ifdef DECLARE_MAINWND
//CMainWindow* mainWnd;
//#else
//extern CMainWindow* mainWnd;
//#endif

