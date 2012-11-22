
#include "AssessmentsPage.h"
#include "StudentEditDlg.h"

CAssessmentsPage::CAssessmentsPage() : CDialog( IDD_ASSESSMENTSPAGE ) {
}

void CAssessmentsPage::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_K2READINGASSESSMENT        , k2ReadingAssessment        );
	//DDX_Control( pDX, IDC_K2READINGBASELINEYEAR      , k2ReadingBaselineYear      );
	//DDX_Control( pDX, IDC_K2READINGBASELINEGRADE     , k2ReadingBaselineGrade     );
	//DDX_Control( pDX, IDC_K2READINGBASELINEACH       , k2ReadingBaselineAch       );
	//DDX_Control( pDX, IDC_K2READINGARDEXP            , k2ReadingARDExp            );
	//DDX_Control( pDX, IDC_K2READINGGRADE             , k2ReadingGrade             );
	//DDX_Control( pDX, IDC_K2READINGACH               , k2ReadingAch               );
	DDX_Control( pDX, IDC_READINGASSESSMENT          , readingAssessment          );
	DDX_Control( pDX, IDC_READINGBASELINEYEAR        , readingBaselineYear        );
	DDX_Control( pDX, IDC_READINGBASELINEGRADE       , readingBaselineGrade       );
	DDX_Control( pDX, IDC_READINGBASELINEACH         , readingBaselineAch         );
	DDX_Control( pDX, IDC_READINGARDEXP              , readingARDExp              );
	DDX_Control( pDX, IDC_READINGGRADE               , readingGrade               );
	DDX_Control( pDX, IDC_READINGACH                 , readingAch                 );
	DDX_Control( pDX, IDC_MATHASSESSMENT             , mathAssessment             );
	DDX_Control( pDX, IDC_MATHBASELINEYEAR           , mathBaselineYear           );
	DDX_Control( pDX, IDC_MATHBASELINEGRADE          , mathBaselineGrade          );
	DDX_Control( pDX, IDC_MATHBASELINEACH            , mathBaselineAch            );
	DDX_Control( pDX, IDC_MATHARDEXP                 , mathARDExp                 );
	DDX_Control( pDX, IDC_MATHGRADE                  , mathGrade                  );
	DDX_Control( pDX, IDC_MATHACH                    , mathAch                    );
	DDX_Control( pDX, IDC_WRITINGASSESSMENT          , writingAssessment          );
	//DDX_Control( pDX, IDC_WRITINGBASELINEYEAR        , writingBaselineYear        );
	//DDX_Control( pDX, IDC_WRITINGBASELINEGRADE       , writingBaselineGrade       );
	//DDX_Control( pDX, IDC_WRITINGBASELINEACH         , writingBaselineAch         );
	DDX_Control( pDX, IDC_WRITINGARDEXP              , writingARDExp              );
	DDX_Control( pDX, IDC_WRITINGGRADE               , writingGrade               );
	DDX_Control( pDX, IDC_WRITINGACH                 , writingAch                 );
	DDX_Control( pDX, IDC_SCIENCEASSESSMENT          , scienceAssessment          );
	DDX_Control( pDX, IDC_SCIENCEBASELINEYEAR        , scienceBaselineYear        );
	DDX_Control( pDX, IDC_SCIENCEBASELINEGRADE       , scienceBaselineGrade       );
	DDX_Control( pDX, IDC_SCIENCEBASELINEACH         , scienceBaselineAch         );
	DDX_Control( pDX, IDC_SCIENCEARDEXP              , scienceARDExp              );
	DDX_Control( pDX, IDC_SCIENCEGRADE               , scienceGrade               );
	DDX_Control( pDX, IDC_SCIENCEACH                 , scienceAch                 );
	DDX_Control( pDX, IDC_SOCIALSTUDIESASSESSMENT    , socialStudiesAssessment    );
	DDX_Control( pDX, IDC_SOCIALSTUDIESBASELINEYEAR  , socialStudiesBaselineYear  );
	DDX_Control( pDX, IDC_SOCIALSTUDIESBASELINEGRADE , socialStudiesBaselineGrade );
	DDX_Control( pDX, IDC_SOCIALSTUDIESBASELINEACH   , socialStudiesBaselineAch   );
	DDX_Control( pDX, IDC_SOCIALSTUDIESARDEXP        , socialStudiesARDExp        );
	DDX_Control( pDX, IDC_SOCIALSTUDIESGRADE         , socialStudiesGrade         );
	DDX_Control( pDX, IDC_SOCIALSTUDIESACH           , socialStudiesAch           );
	DDX_Control( pDX, IDC_ENGLISHLAASSESSMENT        , englishLAAssessment        );
	DDX_Control( pDX, IDC_ENGLISHLABASELINEYEAR      , englishLABaselineYear      );
	DDX_Control( pDX, IDC_ENGLISHLABASELINEGRADE     , englishLABaselineGrade     );
	DDX_Control( pDX, IDC_ENGLISHLABASELINEACH       , englishLABaselineAch       );
	DDX_Control( pDX, IDC_ENGLISHLAARDEXP            , englishLAARDExp            );
	DDX_Control( pDX, IDC_ENGLISHLAGRADE             , englishLAGrade             );
	DDX_Control( pDX, IDC_ENGLISHLAACH               , englishLAAch               );
	/*
	DDX_Control( pDX, IDC_USHISTORYASSESSMENT        , usHistoryAssessment        );
	DDX_Control( pDX, IDC_USHISTORYBASELINEYEAR      , usHistoryBaselineYear      );
	DDX_Control( pDX, IDC_USHISTORYBASELINEGRADE     , usHistoryBaselineGrade     );
	DDX_Control( pDX, IDC_USHISTORYBASELINEACH       , usHistoryBaselineAch       );
	DDX_Control( pDX, IDC_USHISTORYARDEXP            , usHistoryARDExp            );
	DDX_Control( pDX, IDC_USHISTORYGRADE             , usHistoryGrade             );
	DDX_Control( pDX, IDC_USHISTORYACH               , usHistoryAch               );
	DDX_Control( pDX, IDC_ALGEBRAIASSESSMENT         , algebraIAssessment         );
	DDX_Control( pDX, IDC_ALGEBRAIBASELINEYEAR       , algebraIBaselineYear       );
	DDX_Control( pDX, IDC_ALGEBRAIBASELINEGRADE      , algebraIBaselineGrade      );
	DDX_Control( pDX, IDC_ALGEBRAIBASELINEACH        , algebraIBaselineAch        );
	DDX_Control( pDX, IDC_ALGEBRAIARDEXP             , algebraIARDExp             );
	DDX_Control( pDX, IDC_ALGEBRAIGRADE              , algebraIGrade              );
	DDX_Control( pDX, IDC_ALGEBRAIACH                , algebraIAch                );
	DDX_Control( pDX, IDC_BIOLOGYASSESSMENT          , biologyAssessment          );
	DDX_Control( pDX, IDC_BIOLOGYBASELINEYEAR        , biologyBaselineYear        );
	DDX_Control( pDX, IDC_BIOLOGYBASELINEGRADE       , biologyBaselineGrade       );
	DDX_Control( pDX, IDC_BIOLOGYBASELINEACH         , biologyBaselineAch         );
	DDX_Control( pDX, IDC_BIOLOGYARDEXP              , biologyARDExp              );
	DDX_Control( pDX, IDC_BIOLOGYGRADE               , biologyGrade               );
	DDX_Control( pDX, IDC_BIOLOGYACH                 , biologyAch                 );
	*/

	DDX_Control( pDX, IDC_K2READINGMODS, k2ReadingMods );
	DDX_Control( pDX, IDC_READINGMODS, readingMods );
	DDX_Control( pDX, IDC_MATHMODS, mathMods );
	DDX_Control( pDX, IDC_WRITINGMODS, writingMods );
	DDX_Control( pDX, IDC_SCIENCEMODS, scienceMods );
	DDX_Control( pDX, IDC_SOCIALSTUDIESMODS, socialStudiesMods );
	DDX_Control( pDX, IDC_ENGLISHLAMODS, englishLAMods );
	//DDX_Control( pDX, IDC_USHISTORYMODS, usHistoryMods );
	//DDX_Control( pDX, IDC_ALGEBRAIMODS, algebraIMods );
	//DDX_Control( pDX, IDC_BIOLOGYMODS, biologyMods );

	DDX_Control( pDX, IDC_ASSESSMENTARDDATE          , assessmentARDDate          );
	DDX_Control( pDX, IDC_ASSESSMENTSCOMMENTS        , assessmentsComments        );
}

BOOL CAssessmentsPage::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN && (char)(pMsg->wParam) == '\t' ) {
		if ( GetKeyState( VK_SHIFT ) & 1<<31 ) {
			if ( OnShiftTabPressed() ) {
				return CDialog::PreTranslateMessage( pMsg );
			} else {
				return TRUE;
			}
		} else {
			if ( OnTabPressed() ) {
				return CDialog::PreTranslateMessage( pMsg );
			} else {
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage( pMsg );
}

void CAssessmentsPage::OnOK() {
	OnTabPressed();
}

void CAssessmentsPage::OnCancel() {
}

bool CAssessmentsPage::OnTabPressed() {
	if ( GetFocus() == &assessmentsComments ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowNextPage( this );
		return false;
	}
	return true;
}

bool CAssessmentsPage::OnShiftTabPressed() {
	if ( GetFocus() == &k2ReadingAssessment ) {
		CStudentEditDlg* parent = (CStudentEditDlg*)GetParent();
		parent->ShowPreviousPage( this );
		return false;
	}
	return true;
}

