
#include "StudentEditDlg.h"
#include "StorageController.h"
#include "StudentChooserWnd.h"

extern CStorageController storage;
extern CUser user;

void CStudentEditDlg::DoDataExchange( CDataExchange* pDX ) {
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_SSN, ssn );

	DDX_Control( pDX, IDC_FIRSTNAME, firstName );
	DDX_Control( pDX, IDC_MIDDLENAME, middleName );
	DDX_Control( pDX, IDC_LASTNAME, lastName );
	DDX_Control( pDX, IDC_SCHOOLYEAR, schoolYear );

	DDX_Control( pDX, IDC_DEMOGRAPHICSTAB, demographicsTab );
	DDX_Control( pDX, IDC_CONTACTSTAB, contactsTab );
	DDX_Control( pDX, IDC_EVALUATIONSTAB, evaluationsTab );
	DDX_Control( pDX, IDC_ASSESSMENTSTAB, assessmentsTab );
	DDX_Control( pDX, IDC_SERVICESTAB, servicesTab );
	DDX_Control( pDX, IDC_REFERRALSTAB, referralsTab );
	DDX_Control( pDX, IDC_DISTRIBUTIONSTAB, distributionsTab );
}

BOOL CStudentEditDlg::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN && (char)(pMsg->wParam) == '\t' ) {
		OnTabPressed();
	}
	return CDialog::PreTranslateMessage( pMsg );
}

int CStudentEditDlg::OnCreate( LPCREATESTRUCT lpCreateStruct ) {
	int retval = CDialog::OnCreate( lpCreateStruct );
	//dialog.Create( IDD_STUDENTEDIT );

	demographics.Create( IDD_DEMOGRAPHICSPAGE, this );
	contacts.Create( IDD_CONTACTSPAGE, this );
	evaluations.Create( IDD_EVALUATIONSPAGE, this );
	services.Create( IDD_SERVICESPAGE, this );
	assessments.Create( IDD_ASSESSMENTSPAGE, this );
	referrals.Create( IDD_REFERRALSPAGE, this );
	distributions.Create( IDD_DISTRIBUTIONSPAGE, this );

	tabs.push_back( &demographicsTab );
	tabs.push_back( &contactsTab );
	tabs.push_back( &evaluationsTab );
	tabs.push_back( &servicesTab );
	tabs.push_back( &assessmentsTab );
	tabs.push_back( &referralsTab );
	tabs.push_back( &distributionsTab );

	pages.push_back( &demographics );
	pages.push_back( &contacts );
	pages.push_back( &evaluations );
	pages.push_back( &services );
	pages.push_back( &assessments );
	pages.push_back( &referrals );
	pages.push_back( &distributions );

	//dialog.SetWindowPos( &wndTop, 0, 0, 700, 500, SWP_SHOWWINDOW );

	for ( int i=0; i<pages.size(); i++ ) {
		tabs[i]->SetupTabs( tabs, pages );
		pages[i]->SetWindowPos( &wndTop, 11, 84, 577, 320, SWP_SHOWWINDOW );
		pages[i]->ShowWindow( SW_HIDE );
	}
	demographics.ShowWindow( SW_SHOW );

	return retval;
}

void CStudentEditDlg::OnSetFocus( CWnd* pOldWnd ) {
	CDialog::OnSetFocus( pOldWnd );
	lastName.SetFocus();
}

bool CStudentEditDlg::CheckAssessmentIsNull( 
	int assessment,
	vector<int> mods,
	string districtAssessment,
	int baselineYear,
	int baselineGradeLevel,
	int baselineAchLevel,
	int ardExpectation,
	int gradeLevel,
	int achLevel,
	bool& isNull
) {
	string assessmentStr = storage.GetList("Assessment")[ assessment ];

	bool thisIsNull = true;
	if ( assessmentStr != "" && assessmentStr != "Not applicable" ) thisIsNull = false;
	if ( mods.size() ) thisIsNull = false;
	if ( districtAssessment != "" ) thisIsNull = false;
	if ( baselineYear != -1 ) thisIsNull = false;
	if ( baselineGradeLevel != -1 ) thisIsNull = false;
	if ( baselineAchLevel != -1 ) thisIsNull = false;
	if ( ardExpectation != -1 ) thisIsNull = false;
	if ( gradeLevel != -1 ) thisIsNull = false;
	if ( achLevel != -1 ) thisIsNull = false;

	if ( !thisIsNull ) isNull = false;

	if ( !thisIsNull && assessmentStr=="Not applicable" ) {
		MessageBox( "When an assessment is \"Not applicable\", other data cannot be given." );
		return false;
	}

	if ( assessmentStr.length() >= 8 && assessmentStr.substr( assessmentStr.length()-8, 8 ) == "Modified" ) {
		if ( !mods.size() ) {
			MessageBox( "An assessment of \"State-Modified\" or \"SDAA-Modified\" requires a modification." );
			return false;
		}
	}

	return true;
}

#define MB1( a, b, c, d ) \
	MessageBox( ((string)"For " + assessmentDesc + ", when it is " a "a baseline year, " b " must " c "be " d "." ).c_str() )

#define MB2( a, b ) \
	MessageBox( ((string)"When " + assessmentDesc + " is " + assessmentStr + ", " a " must " b "be blank." ).c_str() )

bool CStudentEditDlg::ValidateAssessment( 
	string assessmentDesc,
	int assessment,
	vector<int> mods,
	int baselineYear,
	int baselineGradeLevel,
	int baselineAchLevel,
	int ardExpectation,
	int gradeLevel,
	int achLevel
) {
	string assessmentStr = ( storage.GetList( "Assessment" ) )[ assessment ];

	if ( assessmentStr == "State" || assessmentStr == "State-Modified" || assessmentStr == "District" || assessmentStr == "Exempt" || assessmentStr == "Not applicable" ) {
		if ( baselineYear != -1 ) { MB2( "Baseline Year", "" ); return false; }
		if ( baselineGradeLevel != -1 ) { MB2( "Baseline Grade Level", "" ); return false; }
		if ( baselineAchLevel != -1 ) { MB2( "Baseline Ach. Level", "" ); return false; }
		if ( ardExpectation != -1 ) { MB2( "ARD Expectation", "" ); return false; }
		if ( gradeLevel != -1 ) { MB2( "Grade Level", "" ); return false; }
		if ( achLevel != -1 ) { MB2( "Ach. Level", "" ); return false; }
	}

	if ( assessmentStr == "SDAA" || assessmentStr == "SDAA-Modified" ) {
		if ( baselineYear == -1 ) { MB2( "Baseline Year", "not " ); return false; }
	}

	if ( baselineYear == 1 ) { // "Yes"
		if ( baselineGradeLevel == -1 ) { MB1( "", "Baseline Grade Level", "not ", "blank" ); return false; }
		if ( baselineAchLevel != -1 ) { MB1( "", "Baseline Ach. Level", "", "blank" ); return false; }
		if ( ardExpectation != -1 ) { MB1( "", "ARD Expectation", "", "blank" ); return false; }
		if ( gradeLevel != -1 ) { MB1( "", "Grade Level", "", "blank" ); return false; }
		if ( achLevel != -1 ) { MB1( "", "Ach. Level", "", "blank" ); return false; }
	} else {
		if ( baselineYear == 2 ) { // "No"
			if ( baselineGradeLevel == -1 ) { MB1( "not ", "Baseline Grade Level", "not ", "blank" ); return false; }
			if ( baselineAchLevel == -1 ) { MB1( "not ", "Baseline Ach. Level", "not ", "blank" ); return false; }
			if ( ardExpectation != 1 ) { MB1( "not ", "ARD Expectation", "", "\"Yes\"" ); return false; }
			if ( gradeLevel == -1 ) { MB1( "not ", "Grade Level", "not ", "blank" ); return false; }
			if ( achLevel == -1 ) { MB1( "not ", "Ach. Level", "not ", "blank" ); return false; }
		}
	}

	return true;
}

bool CStudentEditDlg::ValidateStudent() {
#define VA(a,b) \
	if ( !ValidateAssessment( \
		b " Assessment", \
		assessments. ## a ## Assessment.GetSelectedIndex(), \
		assessments. ## a ## Mods.GetMods(), \
		assessments. ## a ## BaselineYear.GetSelectedIndex(), \
		assessments. ## a ## BaselineGrade.GetSelectedIndex(), \
		assessments. ## a ## BaselineAch.GetSelectedIndex(), \
		assessments. ## a ## ARDExp.GetSelectedIndex(), \
		assessments. ## a ## Grade.GetSelectedIndex(), \
		assessments. ## a ## Ach.GetSelectedIndex() \
	) ) { \
		return false; \
	}

	VA(reading, "Reading")
	VA(math, "Math")
	VA(science, "Science")
	VA(socialStudies, "Social Studies")
	VA(englishLA, "English/LA")

	int writingAssessment = assessments.writingAssessment.GetSelectedIndex();
	string writingAssessmentStr = storage.GetList( "Assessment" )[ writingAssessment ];
	if ( writingAssessmentStr.length() >= 4 && writingAssessmentStr.substr(0,4) == "SDAA" ) {
		if ( assessments.writingARDExp.GetSelectedIndex() != 1 ) {
			MessageBox( "When the Writing Assessment is SDAA or SDAA-Modified, ARD Expectation must be \"Yes\"." );
			return false;
		} else {
			if ( assessments.writingGrade.GetSelectedIndex() == -1 ||
				 assessments.writingAch.GetSelectedIndex() == -1 ) {
				MessageBox( "When the Writing Assessment is SDAA or SDAA-Modified, ARD Expectation Grade Level and ARD Expectation Achievement Level must not be blank." );
				return false;
			}
		}
	}

	int notApplicableIndex = storage.GetList("Assessment")[ "Not applicable" ];

#define VG(a,c,b,d) \
	if ( assessments. ## a ## Assessment.GetSelectedIndex() != -1 \
		&& assessments. ## a ## Assessment.GetSelectedIndex() != notApplicableIndex ) { \
		int grade = demographics.grade.GetSelectedIndex(); \
		CIndexedList gradeList = storage.GetList( "Grade" ); \
		string g = gradeList[ grade ]; \
		if ( !( b ) ) { \
			MessageBox( "A " c " Assessment can only be given for students in grades " d "." ); \
			return false; \
		} \
	}

	VG( k2Reading, "K-2 Reading", g=="Kindergarten" || g=="1" || g=="2", "K-2" )
	VG( reading, "Reading", g=="3" || g=="4" || g=="5" || g=="6" || g=="7" || g=="8" || g=="9" , "3-9" )
	VG( math, "Math", g=="3" || g=="4" || g=="5" || g=="6" || g=="7" || g=="8" || g=="9" || g=="10" || g=="11", "3-11" )
	VG( englishLA, "English/LA", g=="10" || g=="11", "10-11" )
	VG( writing, "Writing", g=="4" || g=="7", "4-7" )
	VG( science, "Science", g=="5" || g=="10" || g=="11", "5, 10, and 11" )
	VG( socialStudies, "Social Studies", g=="8" || g=="10" || g=="11", "8, 10, and 11" )

	bool isNull = true;

#define CN(a) \
	if ( !CheckAssessmentIsNull( \
		assessments. ## a ## Assessment.GetSelectedIndex(), \
		assessments. ## a ## Mods.GetMods(), \
		assessments. ## a ## Mods.GetDistrictAssessment(), \
		assessments. ## a ## BaselineYear.GetSelectedIndex(), \
		assessments. ## a ## BaselineGrade.GetSelectedIndex(), \
		assessments. ## a ## BaselineAch.GetSelectedIndex(), \
		assessments. ## a ## ARDExp.GetSelectedIndex(), \
		assessments. ## a ## Grade.GetSelectedIndex(), \
		assessments. ## a ## Ach.GetSelectedIndex(), \
		isNull \
	) ) { \
		return false; \
	}

	CN( reading )
	CN( math )
	CN( science )
	CN( socialStudies )
	CN( englishLA )

	if ( !CheckAssessmentIsNull(
		assessments.k2ReadingAssessment.GetSelectedIndex(), 
		assessments.k2ReadingMods.GetMods(),
		assessments.k2ReadingMods.GetDistrictAssessment(),
		-1, -1, -1, -1, -1, -1, isNull
	) ) {
		return false;
	}

	if ( !CheckAssessmentIsNull(
		assessments.writingAssessment.GetSelectedIndex(), 
		assessments.writingMods.GetMods(),
		assessments.writingMods.GetDistrictAssessment(),
		-1, -1, -1,
		assessments.writingARDExp.GetSelectedIndex(),
		assessments.writingGrade.GetSelectedIndex(),
		assessments.writingAch.GetSelectedIndex(),
		isNull
	) ) {
		return false;
	}

	if ( !isNull ) {
		if ( assessments.assessmentARDDate.GetDate().IsNull() ) {
			MessageBox( "When assessment data has been entered, an ARD date is required." );
			return false;
		}
	}

	return true;
}

bool CStudentEditDlg::AttemptClose() {
	if ( IsModified() ) {
		HRESULT answer = MessageBox( "This student's data has been modified.  Would you like to save it?", ssn.GetString().c_str(), MB_YESNOCANCEL );
		if ( answer == IDCANCEL ) {
			return false;
		} else {
			if ( answer == IDNO ) {
				//DestroyWindow();
				return true;
			} else {
				if ( SaveStudent() ) {
					//DestroyWindow();
					return true;
				} else {
					return false;
				}
			}
		}
	} else {
		return true;
		//CDialog::OnClose();
		//DestroyWindow();
	}
}

void CStudentEditDlg::OnSchoolYearChange() {
	if ( StoreYearData() ) {
		LoadYearData( schoolYear.GetSelectedIndex() );
	} else {
		schoolYear.Select( currentYearID );
	}
}

void CStudentEditDlg::OnOK() {
	OnTabPressed();
}

void CStudentEditDlg::OnCancel() {
	if ( AttemptClose() ) {
		CDialog::OnCancel();
	}
}

void CStudentEditDlg::OnTabPressed() {
	if ( GetFocus() == &schoolYear ) {
		ShowNextPage( this );
	}
}

void CStudentEditDlg::ShowNextPage( CDialog* page ) {
	if ( page == this ) {
		tabs[0]->OnLButtonDown( NULL, CPoint() );
		pages[0]->PostMessage( WM_KEYDOWN, '\t' );
	}

	for ( int i=0; i<pages.size()-1; i++ ) {
		if ( page == pages[i] ) {
			tabs[i+1]->OnLButtonDown( NULL, CPoint() );
			pages[i+1]->PostMessage( WM_KEYDOWN, '\t' );
		}
	}

	if ( page == pages[ pages.size()-1 ] ) {
		tabs[0]->OnLButtonDown( NULL, CPoint() );
		pages[0]->PostMessage( WM_KEYDOWN, '\t' );
		lastName.SetFocus();
	}
}

void CStudentEditDlg::ShowPreviousPage( CDialog* page ) {
	if ( page == this ) {
		tabs[ tabs.size()-1 ]->OnLButtonDown( NULL, CPoint() );
		pages[ pages.size()-1 ]->PostMessage( WM_KEYDOWN, '\t' );
	}

	for ( int i=1; i<pages.size(); i++ ) {
		if ( page == pages[i] ) {
			tabs[i-1]->OnLButtonDown( NULL, CPoint() );
			pages[i-1]->SetFocus();
		}
	}

	if ( page == pages[ 0 ] ) {
		schoolYear.SetFocus();
		//tabs[pages.size()-1]->OnLButtonDown( NULL, CPoint() );
		//pages[pages.size()-1]->PostMessage( WM_KEYDOWN, '\t' );
		//lastName.SetFocus();
	}
}

void CStudentEditDlg::ShowStudent( CStudent& studentData, int yearID ) {
	CConnection conn;
	student = studentData;
	modified = false;

	//Create( IDD_STUDENTEDIT, parent );
	//Create( NULL, "stuff", WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, CRect( 0, 0, 500, 700 ), parent, NULL );
	//SetParent( parent );

	schoolYear.DisableNull();
	referrals.evaluationRequestList.DisableNull();

	schoolYear.SetList( storage.GetList( "SchoolYear", &conn ) );

	demographics.status.SetList( storage.GetList( "Status", &conn ) );
	demographics.eligibility.SetList( storage.GetList( "Eligibility", &conn ) );
	demographics.gender.SetList( storage.GetList( "Gender", &conn ) );
	demographics.ethnicity.SetList( storage.GetList( "Ethnicity", &conn ) );
	demographics.grade.SetList( storage.GetList( "Grade", &conn ) );
	demographics.campus.SetList( storage.GetList( "Campus", &conn ) );

	CIndexedList disabilityList = storage.GetList( "Disability", &conn );
	evaluations.primaryDisability.SetList( disabilityList );
	evaluations.tertiaryDisability.SetList( disabilityList );
	evaluations.secondaryDisability.SetList( disabilityList );

	assessments.k2ReadingAssessment.DisableNull();
	assessments.readingAssessment.DisableNull();
	assessments.mathAssessment.DisableNull();
	assessments.writingAssessment.DisableNull();
	assessments.scienceAssessment.DisableNull();
	assessments.socialStudiesAssessment.DisableNull();
	assessments.englishLAAssessment.DisableNull();

	CIndexedList assessmentList = storage.GetList( "Assessment", &conn );
	assessments.k2ReadingAssessment.SetList( assessmentList );
	assessments.readingAssessment.SetList( assessmentList );
	assessments.mathAssessment.SetList( assessmentList );
	assessments.writingAssessment.SetList( assessmentList );

	CIndexedList otherAssessmentList;
	assessmentList.Rewind();
	while ( assessmentList.HasMoreStrings() ) {
		string a = assessmentList.GetNextString();
		if ( a=="LDAA" || a=="Not applicable" || a=="State" || a=="State-Modified" ) {
			otherAssessmentList.AddString( assessmentList[ a.c_str() ], a );
		}
	}
	assessments.scienceAssessment.SetList( otherAssessmentList );
	assessments.socialStudiesAssessment.SetList( otherAssessmentList );
	assessments.englishLAAssessment.SetList( otherAssessmentList );
	//assessments.usHistoryAssessment.SetList( assessmentList );
	//assessments.algebraIAssessment.SetList( assessmentList );
	//assessments.biologyAssessment.SetList( assessmentList );

	CIndexedList baselineYearList = storage.GetList( "BaselineYear", &conn );
	assessments.readingBaselineYear.SetList( baselineYearList );
	assessments.mathBaselineYear.SetList( baselineYearList );
	//assessments.writingBaselineYear.SetList( baselineYearList );
	assessments.scienceBaselineYear.SetList( baselineYearList );
	assessments.socialStudiesBaselineYear.SetList( baselineYearList );
	assessments.englishLABaselineYear.SetList( baselineYearList );

	CIndexedList ardExpectationList = storage.GetList( "ARDExpectation", &conn );
	assessments.readingARDExp.SetList( ardExpectationList );
	assessments.mathARDExp.SetList( ardExpectationList );
	assessments.writingARDExp.SetList( ardExpectationList );
	assessments.scienceARDExp.SetList( ardExpectationList );
	assessments.socialStudiesARDExp.SetList( ardExpectationList );
	assessments.englishLAARDExp.SetList( ardExpectationList );

	CIndexedList achLevelList = storage.GetList( "AchLevel", &conn );
	//assessments.k2ReadingAch.SetList( achLevelList );
	assessments.readingAch.SetList( achLevelList );
	assessments.mathAch.SetList( achLevelList );
	assessments.writingAch.SetList( achLevelList );
	assessments.scienceAch.SetList( achLevelList );
	assessments.socialStudiesAch.SetList( achLevelList );
	assessments.englishLAAch.SetList( achLevelList );
	//assessments.usHistoryAch.SetList( achLevelList );
	//assessments.algebraIAch.SetList( achLevelList );
	//assessments.biologyAch.SetList( achLevelList );
	//assessments.k2ReadingBaselineAch.SetList( achLevelList );
	assessments.readingBaselineAch.SetList( achLevelList );
	assessments.mathBaselineAch.SetList( achLevelList );
	//assessments.writingBaselineAch.SetList( achLevelList );
	assessments.scienceBaselineAch.SetList( achLevelList );
	assessments.socialStudiesBaselineAch.SetList( achLevelList );
	assessments.englishLABaselineAch.SetList( achLevelList );
	//assessments.usHistoryBaselineAch.SetList( achLevelList );
	//assessments.algebraIBaselineAch.SetList( achLevelList );
	//assessments.biologyBaselineAch.SetList( achLevelList );

	CIndexedList gradeLevelList = storage.GetList( "GradeLevel", &conn );
	//assessments.k2ReadingGrade.SetList( gradeLevelList );
	assessments.readingGrade.SetList( gradeLevelList );
	assessments.mathGrade.SetList( gradeLevelList );
	assessments.scienceGrade.SetList( gradeLevelList );
	assessments.socialStudiesGrade.SetList( gradeLevelList );
	assessments.englishLAGrade.SetList( gradeLevelList );
	//assessments.usHistoryGrade.SetList( gradeLevelList );
	//assessments.algebraIGrade.SetList( gradeLevelList );
	//assessments.biologyGrade.SetList( gradeLevelList );
	//assessments.k2ReadingBaselineGrade.SetList( gradeLevelList );
	assessments.readingBaselineGrade.SetList( gradeLevelList );
	assessments.mathBaselineGrade.SetList( gradeLevelList );
	assessments.scienceBaselineGrade.SetList( gradeLevelList );
	assessments.socialStudiesBaselineGrade.SetList( gradeLevelList );
	assessments.englishLABaselineGrade.SetList( gradeLevelList );
	//assessments.usHistoryBaselineGrade.SetList( gradeLevelList );
	//assessments.algebraIBaselineGrade.SetList( gradeLevelList );
	//assessments.biologyBaselineGrade.SetList( gradeLevelList );

	CIndexedList writingGradeLevelList;
	gradeLevelList.Rewind();
	while ( gradeLevelList.HasMoreStrings() ) {
		string s = gradeLevelList.GetNextString();
		if ( (int)s.find( '-' ) >= 0 ) {
			writingGradeLevelList.AddString( gradeLevelList[s.c_str()], s );
		}
	}
	assessments.writingGrade.SetList( writingGradeLevelList );
	//assessments.writingBaselineGrade.SetList( writingGradeLevelList );

	services.instructionalSetting.SetList( storage.GetList( "InstructionalSetting", &conn ) );

	referrals.referralEligible.SetList( storage.GetList( "ReferralEligible", &conn ) );
	referrals.referringCampus.SetList( storage.GetList( "Campus", &conn ) );
	referrals.evaluationRequestList.SetList( storage.GetList( "EvaluationType", &conn ) );

	CIndexedList serviceTypeList = storage.GetList( "ServiceType", &conn );

	ssn.SetString( student.ssn );
	firstName.SetString( student.self.firstName );
	middleName.SetString( student.self.middleName );
	lastName.SetString( student.self.lastName );
	//schoolYear.Select( yearID );

	contacts.motherFirstName.SetString( student.mother.firstName );
	contacts.motherLastName.SetString( student.mother.lastName );
	contacts.motherAddress.SetString( student.mother.address );
	contacts.motherCity.SetString( student.mother.city );
	contacts.motherState.SetString( student.mother.state );
	contacts.motherZipCode.SetString( student.mother.zipCode );
	contacts.motherWorkPhone.SetString( student.mother.workPhone );
	contacts.motherHomePhone.SetString( student.mother.homePhone );
	contacts.motherCellPhone.SetString( student.mother.cellPhone );
	contacts.motherEmail.SetString( student.mother.emailAddress );

	contacts.fatherFirstName.SetString( student.father.firstName );
	contacts.fatherLastName.SetString( student.father.lastName );
	contacts.fatherAddress.SetString( student.father.address );
	contacts.fatherCity.SetString( student.father.city );
	contacts.fatherState.SetString( student.father.state );
	contacts.fatherZipCode.SetString( student.father.zipCode );
	contacts.fatherWorkPhone.SetString( student.father.workPhone );
	contacts.fatherHomePhone.SetString( student.father.homePhone );
	contacts.fatherCellPhone.SetString( student.mother.cellPhone );
	contacts.fatherEmail.SetString( student.mother.emailAddress );

	demographics.address.SetString( student.self.address );
	demographics.city.SetString( student.self.city );
	demographics.state.SetString( student.self.state );
	demographics.zipCode.SetString( student.self.zipCode );
	demographics.mailingAddress.SetString( student.self.mailingAddress );
	demographics.mailingCity.SetString( student.self.mailingCity );
	demographics.mailingState.SetString( student.self.mailingState );
	demographics.mailingZipCode.SetString( student.self.mailingZipCode );
	demographics.homePhone.SetString( student.self.homePhone );

	distributions.distributions.AddColumn( "Date", new CDateEditHelper() );
	distributions.distributions.AddColumn( "Document", new CIndexedListBoxHelper( storage.GetList( "Document", &conn ) ) );
	distributions.distributions.AddColumn( "Method", new CIndexedListBoxHelper( storage.GetList( "DistributionMethod", &conn ) ) );
	distributions.distributions.AddColumn( "Initial", new CIndexedListBoxHelper( storage.GetList( "InitialDistribution", &conn ) ) );
	evaluations.evaluations.AddColumn( "Evaluation Type", new CIndexedListBoxHelper( storage.GetList( "EvaluationType", &conn ) ) );
	evaluations.evaluations.AddColumn( "Evaluation Date", new CDateEditHelper() );
	evaluations.evaluations.AddColumn( "Report Date", new CDateEditHelper() );
	evaluations.evaluations.AddColumn( "Eligible", new CIndexedListBoxHelper( storage.GetList( "EvaluationEligible", &conn ) ) );
	evaluations.evaluations.AddColumn( "Dismissal Date", new CDateEditHelper() );
	services.instructionalServices.AddColumn( "Instructional Service", new CIndexedListBoxHelper( storage.GetList( "InstructionalServiceType", &conn ) ) );
	services.instructionalServices.AddColumn( "Type", new CIndexedListBoxHelper( serviceTypeList ) );
	services.instructionalServices.AddColumn( "Time", new CStringEditHelper() );
	services.relatedServices.AddColumn( "Related Service", new CIndexedListBoxHelper( storage.GetList( "RelatedServiceType", &conn ) ) );
	services.relatedServices.AddColumn( "Type", new CIndexedListBoxHelper( serviceTypeList ) );
	services.relatedServices.AddColumn( "Time", new CStringEditHelper() );
	services.esyServices.AddColumn( "ESY Service", new CIndexedListBoxHelper( storage.GetList( "ESYServiceType", &conn ) ) );
	services.esyServices.AddColumn( "Type", new CIndexedListBoxHelper( serviceTypeList ) );
	services.esyServices.AddColumn( "Time", new CStringEditHelper() );

	//int maxYearID=-1;
	//for ( int i=0; i<student.years.size(); i++ ) {
	//	if ( student.years[i].schoolYearID > maxYearID ) {
	//		maxYearID = student.years[i].schoolYearID;
	//	}
	//}

	//if ( maxYearID != -1 ) {
	//	LoadYearData( maxYearID );
	//}

	LoadYearData( yearID );

	conn.Close();

	if ( !user.CanWrite() ) {
		SetReadOnly();
	}

	//SetWindowText( student.ssn.c_str() );

	//ShowWindow( SW_SHOW );
	//UpdateWindow();
}

void CStudentEditDlg::LoadYearData( int yearID ) {
	bool found = false;

	schoolYear.Select( yearID );
	for ( int j=0; j<student.years.size() && !found; j++ ) {
		if ( yearID == student.years[j].schoolYearID ) {
			currentYearID = yearID;
			found = true;
			demographics.birthDate.SetDate( student.years[j].birthDate );
			demographics.statusDate.SetDate( student.years[j].statusDate );

			demographics.status.Select( student.years[j].status );
			demographics.eligibility.Select( student.years[j].eligibility );
			demographics.gender.Select( student.years[j].gender );
			demographics.ethnicity.Select( student.years[j].ethnicity );
			demographics.campus.Select( student.years[j].campus );
			demographics.grade.Select( student.years[j].grade );

			evaluations.primaryDisability.Select( student.years[j].primaryDisability );
			evaluations.secondaryDisability.Select( student.years[j].secondaryDisability );
			evaluations.tertiaryDisability.Select( student.years[j].tertiaryDisability );
			evaluations.primaryDisabilityCode.SetIndex( student.years[j].primaryDisability );
			evaluations.secondaryDisabilityCode.SetIndex( student.years[j].secondaryDisability );
			evaluations.tertiaryDisabilityCode.SetIndex( student.years[j].tertiaryDisability );
			evaluations.multipleDisabilities.SetValue( student.years[j].multipleDisabilities );

			evaluations.evaluations.ClearRows();
			for ( int i=0; i<student.years[j].evaluations.size(); i++ ) {
				vector< CVariant > evalData;
				evalData.push_back( student.years[j].evaluations[i].evaluationType );
				evalData.push_back( student.years[j].evaluations[i].evaluationDate );
				evalData.push_back( student.years[j].evaluations[i].reportDate );
				evalData.push_back( student.years[j].evaluations[i].evaluationEligible );
				evalData.push_back( student.years[j].evaluations[i].dismissalDate );
				evaluations.evaluations.AddRow( evalData );
			}
			evaluations.evaluations.AutoSizeColumns();

			services.instructionalSetting.Select( student.years[j].instructionalSetting );
			services.instructionalSettingCode.SetIndex( student.years[j].instructionalSetting );
			services.lastARD.SetDate( student.years[j].lastARDDate );
			services.lastAnnualReview.SetDate( student.years[j].lastAnnualReviewDate );
			services.itpDate.SetDate( student.years[j].itpDate );
			services.iepStartDate.SetDate( student.years[j].iepStartDate );
			services.iepEndDate.SetDate( student.years[j].iepEndDate );
			services.medicallyFragile.SetValue( student.years[j].medicallyFragile );
			services.medicaid.SetValue( student.years[j].medicaid );

			services.instructionalServices.ClearRows();
			for ( i=0; i<student.years[j].instructionalServices.size(); i++ ) {
				vector< CVariant > serviceData;
				serviceData.push_back( student.years[j].instructionalServices[i].specificServiceType );
				serviceData.push_back( student.years[j].instructionalServices[i].serviceType );
				serviceData.push_back( student.years[j].instructionalServices[i].serviceTime );
				services.instructionalServices.AddRow( serviceData );
			}
			services.instructionalServices.AutoSizeColumns();

			services.relatedServices.ClearRows();
			for ( i=0; i<student.years[j].relatedServices.size(); i++ ) {
				vector< CVariant > serviceData;
				serviceData.push_back( student.years[j].relatedServices[i].specificServiceType );
				serviceData.push_back( student.years[j].relatedServices[i].serviceType );
				serviceData.push_back( student.years[j].relatedServices[i].serviceTime );
				services.relatedServices.AddRow( serviceData );
			}
			services.relatedServices.AutoSizeColumns();

			services.esyServices.ClearRows();
			for ( i=0; i<student.years[j].esyServices.size(); i++ ) {
				vector< CVariant > serviceData;
				serviceData.push_back( student.years[j].esyServices[i].specificServiceType );
				serviceData.push_back( student.years[j].esyServices[i].serviceType );
				serviceData.push_back( student.years[j].esyServices[i].serviceTime );
				services.esyServices.AddRow( serviceData );
			}
			services.esyServices.AutoSizeColumns();

			assessments.k2ReadingAssessment.Select( student.years[j].k2ReadingAssessmentRecord.assessment );
			//assessments.k2ReadingBaselineYear.SetValue( student.years[j].k2ReadingAssessmentRecord.baselineYear );
			//assessments.k2ReadingBaselineGrade.Select( student.years[j].k2ReadingAssessmentRecord.baselineGradeLevel );
			//assessments.k2ReadingBaselineAch.Select( student.years[j].k2ReadingAssessmentRecord.baselineAchLevel );
			//assessments.k2ReadingARDExp.SetValue( student.years[j].k2ReadingAssessmentRecord.ardExpectation );
			//assessments.k2ReadingGrade.Select( student.years[j].k2ReadingAssessmentRecord.gradeLevel );
			//assessments.k2ReadingAch.Select( student.years[j].k2ReadingAssessmentRecord.achLevel );

			assessments.readingAssessment.Select( student.years[j].readingAssessmentRecord.assessment );
			assessments.readingBaselineYear.Select( student.years[j].readingAssessmentRecord.baselineYear );
			assessments.readingBaselineGrade.Select( student.years[j].readingAssessmentRecord.baselineGradeLevel );
			assessments.readingBaselineAch.Select( student.years[j].readingAssessmentRecord.baselineAchLevel );
			assessments.readingARDExp.Select( student.years[j].readingAssessmentRecord.ardExpectation );
			assessments.readingGrade.Select( student.years[j].readingAssessmentRecord.gradeLevel );
			assessments.readingAch.Select( student.years[j].readingAssessmentRecord.achLevel );

			assessments.mathAssessment.Select( student.years[j].mathAssessmentRecord.assessment );
			assessments.mathBaselineYear.Select( student.years[j].mathAssessmentRecord.baselineYear );
			assessments.mathBaselineGrade.Select( student.years[j].mathAssessmentRecord.baselineGradeLevel );
			assessments.mathBaselineAch.Select( student.years[j].mathAssessmentRecord.baselineAchLevel );
			assessments.mathARDExp.Select( student.years[j].mathAssessmentRecord.ardExpectation );
			assessments.mathGrade.Select( student.years[j].mathAssessmentRecord.gradeLevel );
			assessments.mathAch.Select( student.years[j].mathAssessmentRecord.achLevel );

			assessments.writingAssessment.Select( student.years[j].writingAssessmentRecord.assessment );
			//assessments.writingBaselineYear.Select( student.years[j].writingAssessmentRecord.baselineYear );
			//assessments.writingBaselineGrade.Select( student.years[j].writingAssessmentRecord.baselineGradeLevel );
			//assessments.writingBaselineAch.Select( student.years[j].writingAssessmentRecord.baselineAchLevel );
			assessments.writingARDExp.Select( student.years[j].writingAssessmentRecord.ardExpectation );
			assessments.writingGrade.Select( student.years[j].writingAssessmentRecord.gradeLevel );
			assessments.writingAch.Select( student.years[j].writingAssessmentRecord.achLevel );

			assessments.scienceAssessment.Select( student.years[j].scienceAssessmentRecord.assessment );
			assessments.scienceBaselineYear.Select( student.years[j].scienceAssessmentRecord.baselineYear );
			assessments.scienceBaselineGrade.Select( student.years[j].scienceAssessmentRecord.baselineGradeLevel );
			assessments.scienceBaselineAch.Select( student.years[j].scienceAssessmentRecord.baselineAchLevel );
			assessments.scienceARDExp.Select( student.years[j].scienceAssessmentRecord.ardExpectation );
			assessments.scienceGrade.Select( student.years[j].scienceAssessmentRecord.gradeLevel );
			assessments.scienceAch.Select( student.years[j].scienceAssessmentRecord.achLevel );

			assessments.socialStudiesAssessment.Select( student.years[j].socialStudiesAssessmentRecord.assessment );
			assessments.socialStudiesBaselineYear.Select( student.years[j].socialStudiesAssessmentRecord.baselineYear );
			assessments.socialStudiesBaselineGrade.Select( student.years[j].socialStudiesAssessmentRecord.baselineGradeLevel );
			assessments.socialStudiesBaselineAch.Select( student.years[j].socialStudiesAssessmentRecord.baselineAchLevel );
			assessments.socialStudiesARDExp.Select( student.years[j].socialStudiesAssessmentRecord.ardExpectation );
			assessments.socialStudiesGrade.Select( student.years[j].socialStudiesAssessmentRecord.gradeLevel );
			assessments.socialStudiesAch.Select( student.years[j].socialStudiesAssessmentRecord.achLevel );

			assessments.englishLAAssessment.Select( student.years[j].englishLAAssessmentRecord.assessment );
			assessments.englishLABaselineYear.Select( student.years[j].englishLAAssessmentRecord.baselineYear );
			assessments.englishLABaselineGrade.Select( student.years[j].englishLAAssessmentRecord.baselineGradeLevel );
			assessments.englishLABaselineAch.Select( student.years[j].englishLAAssessmentRecord.baselineAchLevel );
			assessments.englishLAARDExp.Select( student.years[j].englishLAAssessmentRecord.ardExpectation );
			assessments.englishLAGrade.Select( student.years[j].englishLAAssessmentRecord.gradeLevel );
			assessments.englishLAAch.Select( student.years[j].englishLAAssessmentRecord.achLevel );

			assessments.k2ReadingMods.SetMods( student.years[j].k2ReadingAssessmentRecord.mods );
			assessments.readingMods.SetMods( student.years[j].readingAssessmentRecord.mods );
			assessments.mathMods.SetMods( student.years[j].mathAssessmentRecord.mods );
			assessments.writingMods.SetMods( student.years[j].writingAssessmentRecord.mods );
			assessments.scienceMods.SetMods( student.years[j].scienceAssessmentRecord.mods );
			assessments.socialStudiesMods.SetMods( student.years[j].socialStudiesAssessmentRecord.mods );
			assessments.englishLAMods.SetMods( student.years[j].englishLAAssessmentRecord.mods );

			assessments.k2ReadingMods.SetDistrictAssessment( student.years[j].k2ReadingAssessmentRecord.districtAssessment );
			assessments.readingMods.SetDistrictAssessment( student.years[j].readingAssessmentRecord.districtAssessment );
			assessments.mathMods.SetDistrictAssessment( student.years[j].mathAssessmentRecord.districtAssessment );
			assessments.writingMods.SetDistrictAssessment( student.years[j].writingAssessmentRecord.districtAssessment );
			assessments.scienceMods.SetDistrictAssessment( student.years[j].scienceAssessmentRecord.districtAssessment );
			assessments.socialStudiesMods.SetDistrictAssessment( student.years[j].socialStudiesAssessmentRecord.districtAssessment );
			assessments.englishLAMods.SetDistrictAssessment( student.years[j].englishLAAssessmentRecord.districtAssessment );

			assessments.assessmentARDDate.SetDate(student.years[j].assessmentARDDate);

			referrals.referralDate.SetDate( student.years[j].referralDate );
			referrals.referringCampus.Select( student.years[j].referringCampus );
			referrals.evaluationCompletedDate.SetDate( student.years[j].evaluationCompletedDate );
			referrals.ardCompletedDate.SetDate( student.years[j].ardCompletedDate );
			referrals.referralEligible.Select( student.years[j].referralEligible );

			referrals.evaluationsRequested.ResetContent();
			for ( i=0; i<student.years[j].evaluationsRequested.size(); i++ ) {
				CIndexedList* evalList = referrals.evaluationRequestList.GetList();
				int evalID = student.years[j].evaluationsRequested[i];
				string evalDesc = (*evalList)[ evalID ];
				referrals.evaluationsRequested.AddString( evalDesc.c_str() );
			}

			demographics.demographicsComments.SetString( student.years[j].demographicsComments );
			contacts.contactsComments.SetString( student.years[j].contactsComments );
			evaluations.evaluationsComments.SetString( student.years[j].evaluationsComments );
			assessments.assessmentsComments.SetString( student.years[j].assessmentsComments );
			services.servicesComments.SetString( student.years[j].servicesComments );
			referrals.referralsComments.SetString( student.years[j].referralsComments );
			distributions.distributionsComments.SetString( student.years[j].distributionsComments );

			distributions.distributions.ClearRows();
			for ( i=0; i<student.years[j].distributions.size(); i++ ) {
				vector< CVariant > distData;
				distData.push_back( student.years[j].distributions[i].distributionDate );
				distData.push_back( student.years[j].distributions[i].documentID );
				distData.push_back( student.years[j].distributions[i].distributionMethodID );
				distData.push_back( student.years[j].distributions[i].initialDistributionID );
				distributions.distributions.AddRow( distData );
			}
			distributions.distributions.AutoSizeColumns();
		}
	}

	if ( !found ) {
		if ( !user.CanWrite() ) {
			MessageBox( "This student has no record for that school year.", "StudentInfo" );
			schoolYear.Select( currentYearID );
		} else {
			HRESULT ans = MessageBox( "This student has no record for this year.\nWould you like to create one?", "", MB_YESNO );
			if ( ans == IDYES ) {
				currentYearID = yearID;
				StoreYearData( false );
			} else {
				schoolYear.Select( currentYearID );
			}
		}
	}
}

bool CStudentEditDlg::StoreYearData( bool checkModified ) {
	int i,j;

	if ( !ValidateStudent() ) {
		return false;
	}

	if ( !checkModified || IsYearDataModified() ) {

		evaluations.evaluations.EndEdit();
		services.instructionalServices.EndEdit();
		services.esyServices.EndEdit();
		services.relatedServices.EndEdit();
		distributions.distributions.EndEdit();

		modified = true;
		int yearIndex;
		bool found = false;
		for ( j=0; j<student.years.size() && !found; j++ ) {
			if ( student.years[j].schoolYearID == currentYearID ) {
				yearIndex = j;
				found = true;
			}
		}
		if ( !found ) {
			CStudentYear studentYear;
			studentYear.schoolYearID = currentYearID;
			student.years.push_back( studentYear );
			yearIndex = student.years.size()-1;
		}

		student.years[yearIndex].birthDate = demographics.birthDate.GetDate();
		student.years[yearIndex].statusDate = demographics.statusDate.GetDate();

		student.years[yearIndex].status = demographics.status.GetSelectedIndex();
		student.years[yearIndex].eligibility = demographics.eligibility.GetSelectedIndex();
		student.years[yearIndex].gender = demographics.gender.GetSelectedIndex();
		student.years[yearIndex].ethnicity = demographics.ethnicity.GetSelectedIndex();
		student.years[yearIndex].campus = demographics.campus.GetSelectedIndex();
		student.years[yearIndex].grade = demographics.grade.GetSelectedIndex();

		student.years[yearIndex].primaryDisability = evaluations.primaryDisability.GetSelectedIndex();
		student.years[yearIndex].secondaryDisability = evaluations.secondaryDisability.GetSelectedIndex();
		student.years[yearIndex].tertiaryDisability = evaluations.tertiaryDisability.GetSelectedIndex();
		student.years[yearIndex].multipleDisabilities = evaluations.multipleDisabilities.GetValue();

		student.years[yearIndex].evaluations.clear();
		for ( i=0; i<evaluations.evaluations.GetRowCount()-1; i++ ) {
			CEvaluation evalData;
			evalData.evaluationType     = evaluations.evaluations[ CGridLocation( i,0 ) ];
			evalData.evaluationDate     = evaluations.evaluations[ CGridLocation( i,1 ) ];
			evalData.reportDate         = evaluations.evaluations[ CGridLocation( i,2 ) ];
			evalData.evaluationEligible = evaluations.evaluations[ CGridLocation( i,3 ) ];
			evalData.dismissalDate      = evaluations.evaluations[ CGridLocation( i,4 ) ];
			student.years[yearIndex].evaluations.push_back( evalData );
		}

		student.years[yearIndex].instructionalSetting = services.instructionalSetting.GetSelectedIndex();
		student.years[yearIndex].lastARDDate = services.lastARD.GetDate();
		student.years[yearIndex].lastAnnualReviewDate = services.lastAnnualReview.GetDate();
		student.years[yearIndex].itpDate = services.itpDate.GetDate();
		student.years[yearIndex].iepStartDate = services.iepStartDate.GetDate();
		student.years[yearIndex].iepEndDate = services.iepEndDate.GetDate();
		student.years[yearIndex].medicallyFragile = services.medicallyFragile.GetValue();
		student.years[yearIndex].medicaid = services.medicaid.GetValue();

		student.years[yearIndex].instructionalServices.clear();
		for ( i=0; i<services.instructionalServices.GetRowCount()-1; i++ ) {
			CService serviceData;
			serviceData.specificServiceType = services.instructionalServices[ CGridLocation( i,0 ) ];
			serviceData.serviceType = services.instructionalServices[ CGridLocation( i,1 ) ];
			serviceData.serviceTime = services.instructionalServices[ CGridLocation( i,2 ) ];
			student.years[yearIndex].instructionalServices.push_back( serviceData );
		}

		student.years[yearIndex].relatedServices.clear();
		for ( i=0; i<services.relatedServices.GetRowCount()-1; i++ ) {
			CService serviceData;
			serviceData.specificServiceType = services.relatedServices[ CGridLocation( i,0 ) ];
			serviceData.serviceType = services.relatedServices[ CGridLocation( i,1 ) ];
			serviceData.serviceTime = services.relatedServices[ CGridLocation( i,2 ) ];
			student.years[yearIndex].relatedServices.push_back( serviceData );
		}

		student.years[yearIndex].esyServices.clear();
		for ( i=0; i<services.esyServices.GetRowCount()-1; i++ ) {
			CService serviceData;
			serviceData.specificServiceType = services.esyServices[ CGridLocation( i,0 ) ];
			serviceData.serviceType = services.esyServices[ CGridLocation( i,1 ) ];
			serviceData.serviceTime = services.esyServices[ CGridLocation( i,2 ) ];
			student.years[yearIndex].esyServices.push_back( serviceData );
		}

		student.years[yearIndex].evaluationsRequested.clear();
		CIndexedList* evalList = referrals.evaluationRequestList.GetList();
		for ( i=0; i<referrals.evaluationsRequested.GetCount(); i++ ) {
			CString str;
			referrals.evaluationsRequested.GetText( i, str );
			int evalID = (*evalList)[ (LPCTSTR)str ];
			student.years[yearIndex].evaluationsRequested.push_back( evalID );
		}

		student.years[yearIndex].k2ReadingAssessmentRecord.assessment = assessments.k2ReadingAssessment.GetSelectedIndex();
		student.years[yearIndex].readingAssessmentRecord.assessment = assessments.readingAssessment.GetSelectedIndex();
		student.years[yearIndex].mathAssessmentRecord.assessment = assessments.mathAssessment.GetSelectedIndex();
		student.years[yearIndex].writingAssessmentRecord.assessment = assessments.writingAssessment.GetSelectedIndex();
		student.years[yearIndex].scienceAssessmentRecord.assessment = assessments.scienceAssessment.GetSelectedIndex();
		student.years[yearIndex].socialStudiesAssessmentRecord.assessment = assessments.socialStudiesAssessment.GetSelectedIndex();
		student.years[yearIndex].englishLAAssessmentRecord.assessment = assessments.englishLAAssessment.GetSelectedIndex();

		student.years[yearIndex].readingAssessmentRecord.baselineYear = assessments.readingBaselineYear.GetSelectedIndex();
		student.years[yearIndex].mathAssessmentRecord.baselineYear = assessments.mathBaselineYear.GetSelectedIndex();
		student.years[yearIndex].scienceAssessmentRecord.baselineYear = assessments.scienceBaselineYear.GetSelectedIndex();
		student.years[yearIndex].socialStudiesAssessmentRecord.baselineYear = assessments.socialStudiesBaselineYear.GetSelectedIndex();
		student.years[yearIndex].englishLAAssessmentRecord.baselineYear = assessments.englishLABaselineYear.GetSelectedIndex();

		student.years[yearIndex].readingAssessmentRecord.baselineGradeLevel = assessments.readingBaselineGrade.GetSelectedIndex();
		student.years[yearIndex].mathAssessmentRecord.baselineGradeLevel = assessments.mathBaselineGrade.GetSelectedIndex();
		student.years[yearIndex].scienceAssessmentRecord.baselineGradeLevel = assessments.scienceBaselineGrade.GetSelectedIndex();
		student.years[yearIndex].socialStudiesAssessmentRecord.baselineGradeLevel = assessments.socialStudiesBaselineGrade.GetSelectedIndex();
		student.years[yearIndex].englishLAAssessmentRecord.baselineGradeLevel = assessments.englishLABaselineGrade.GetSelectedIndex();

		student.years[yearIndex].readingAssessmentRecord.baselineAchLevel = assessments.readingBaselineAch.GetSelectedIndex();
		student.years[yearIndex].mathAssessmentRecord.baselineAchLevel = assessments.mathBaselineAch.GetSelectedIndex();
		student.years[yearIndex].scienceAssessmentRecord.baselineAchLevel = assessments.scienceBaselineAch.GetSelectedIndex();
		student.years[yearIndex].socialStudiesAssessmentRecord.baselineAchLevel = assessments.socialStudiesBaselineAch.GetSelectedIndex();
		student.years[yearIndex].englishLAAssessmentRecord.baselineAchLevel = assessments.englishLABaselineAch.GetSelectedIndex();

		student.years[yearIndex].readingAssessmentRecord.ardExpectation = assessments.readingARDExp.GetSelectedIndex();
		student.years[yearIndex].mathAssessmentRecord.ardExpectation = assessments.mathARDExp.GetSelectedIndex();
		student.years[yearIndex].writingAssessmentRecord.ardExpectation = assessments.writingARDExp.GetSelectedIndex();
		student.years[yearIndex].scienceAssessmentRecord.ardExpectation = assessments.scienceARDExp.GetSelectedIndex();
		student.years[yearIndex].socialStudiesAssessmentRecord.ardExpectation = assessments.socialStudiesARDExp.GetSelectedIndex();
		student.years[yearIndex].englishLAAssessmentRecord.ardExpectation = assessments.englishLAARDExp.GetSelectedIndex();

		student.years[yearIndex].readingAssessmentRecord.gradeLevel = assessments.readingGrade.GetSelectedIndex();
		student.years[yearIndex].mathAssessmentRecord.gradeLevel = assessments.mathGrade.GetSelectedIndex();
		student.years[yearIndex].writingAssessmentRecord.gradeLevel = assessments.writingGrade.GetSelectedIndex();
		student.years[yearIndex].scienceAssessmentRecord.gradeLevel = assessments.scienceGrade.GetSelectedIndex();
		student.years[yearIndex].socialStudiesAssessmentRecord.gradeLevel = assessments.socialStudiesGrade.GetSelectedIndex();
		student.years[yearIndex].englishLAAssessmentRecord.gradeLevel = assessments.englishLAGrade.GetSelectedIndex();

		student.years[yearIndex].readingAssessmentRecord.achLevel = assessments.readingAch.GetSelectedIndex();
		student.years[yearIndex].mathAssessmentRecord.achLevel = assessments.mathAch.GetSelectedIndex();
		student.years[yearIndex].writingAssessmentRecord.achLevel = assessments.writingAch.GetSelectedIndex();
		student.years[yearIndex].scienceAssessmentRecord.achLevel = assessments.scienceAch.GetSelectedIndex();
		student.years[yearIndex].socialStudiesAssessmentRecord.achLevel = assessments.socialStudiesAch.GetSelectedIndex();
		student.years[yearIndex].englishLAAssessmentRecord.achLevel = assessments.englishLAAch.GetSelectedIndex();

		student.years[yearIndex].k2ReadingAssessmentRecord.mods = assessments.k2ReadingMods.GetMods();
		student.years[yearIndex].readingAssessmentRecord.mods = assessments.readingMods.GetMods();
		student.years[yearIndex].mathAssessmentRecord.mods = assessments.mathMods.GetMods();
		student.years[yearIndex].writingAssessmentRecord.mods = assessments.writingMods.GetMods();
		student.years[yearIndex].scienceAssessmentRecord.mods = assessments.scienceMods.GetMods();
		student.years[yearIndex].socialStudiesAssessmentRecord.mods = assessments.socialStudiesMods.GetMods();
		student.years[yearIndex].englishLAAssessmentRecord.mods = assessments.englishLAMods.GetMods();

		student.years[yearIndex].k2ReadingAssessmentRecord.districtAssessment = assessments.k2ReadingMods.GetDistrictAssessment();
		student.years[yearIndex].readingAssessmentRecord.districtAssessment = assessments.readingMods.GetDistrictAssessment();
		student.years[yearIndex].mathAssessmentRecord.districtAssessment = assessments.mathMods.GetDistrictAssessment();
		student.years[yearIndex].writingAssessmentRecord.districtAssessment = assessments.writingMods.GetDistrictAssessment();
		student.years[yearIndex].scienceAssessmentRecord.districtAssessment = assessments.scienceMods.GetDistrictAssessment();
		student.years[yearIndex].socialStudiesAssessmentRecord.districtAssessment = assessments.socialStudiesMods.GetDistrictAssessment();
		student.years[yearIndex].englishLAAssessmentRecord.districtAssessment = assessments.englishLAMods.GetDistrictAssessment();

		student.years[yearIndex].assessmentARDDate = assessments.assessmentARDDate.GetDate();

		student.years[yearIndex].referralDate = referrals.referralDate.GetDate();
		student.years[yearIndex].referringCampus = referrals.referringCampus.GetSelectedIndex();
		student.years[yearIndex].evaluationCompletedDate = referrals.evaluationCompletedDate.GetDate();
		student.years[yearIndex].ardCompletedDate = referrals.ardCompletedDate.GetDate();
		student.years[yearIndex].referralEligible = referrals.referralEligible.GetSelectedIndex();

		student.years[yearIndex].demographicsComments = demographics.demographicsComments.GetString();
		student.years[yearIndex].contactsComments = contacts.contactsComments.GetString();
		student.years[yearIndex].evaluationsComments = evaluations.evaluationsComments.GetString();
		student.years[yearIndex].assessmentsComments = assessments.assessmentsComments.GetString();
		student.years[yearIndex].servicesComments = services.servicesComments.GetString();
		student.years[yearIndex].referralsComments = referrals.referralsComments.GetString();
		student.years[yearIndex].distributionsComments = distributions.distributionsComments.GetString();

		student.years[yearIndex].distributions.clear();
		for ( i=0; i<distributions.distributions.GetRowCount()-1; i++ ) {
			CDistribution distData;
			distData.distributionDate = distributions.distributions[ CGridLocation( i,0 ) ];
			distData.documentID = distributions.distributions[ CGridLocation( i,1 ) ];
			distData.distributionMethodID = distributions.distributions[ CGridLocation( i,2 ) ];
			distData.initialDistributionID = distributions.distributions[ CGridLocation( i,3 ) ];
			student.years[yearIndex].distributions.push_back( distData );
		}
	}

	return true;
}

bool CStudentEditDlg::SaveStudent() {
	if ( !user.CanWrite() ) {
		return false;
	}

	ssn.ValidateData();
	if ( ssn.GetString() == "" ) {
		MessageBox( "The SSN cannot be blank.", "" );
		return false;
	}
	//if ( IsModified() ) {
		//CStudent student( schoolYear.GetSelectedIndex() );
		evaluations.evaluations.EndEdit();
		services.instructionalServices.EndEdit();
		services.relatedServices.EndEdit();
		services.esyServices.EndEdit();
	
		student.ssn = ssn.GetString();
		student.self.firstName = firstName.GetString(  );
		student.self.middleName = middleName.GetString(  );
		student.self.lastName = lastName.GetString(  );
		//schoolYear.Select( yearID );

		student.mother.firstName = contacts.motherFirstName.GetString();
		student.mother.lastName = contacts.motherLastName.GetString();
		student.mother.address = contacts.motherAddress.GetString();
		student.mother.city = contacts.motherCity.GetString();
		student.mother.state = contacts.motherState.GetString();
		student.mother.zipCode = contacts.motherZipCode.GetString();
		student.mother.workPhone = contacts.motherWorkPhone.GetString();
		student.mother.homePhone = contacts.motherHomePhone.GetString();
		student.mother.cellPhone = contacts.motherCellPhone.GetString();
		student.mother.emailAddress = contacts.motherEmail.GetString();

		student.father.firstName = contacts.fatherFirstName.GetString();
		student.father.lastName = contacts.fatherLastName.GetString();
		student.father.address = contacts.fatherAddress.GetString();
		student.father.city = contacts.fatherCity.GetString();
		student.father.state = contacts.fatherState.GetString();
		student.father.zipCode = contacts.fatherZipCode.GetString();
		student.father.workPhone = contacts.fatherWorkPhone.GetString();
		student.father.homePhone = contacts.fatherHomePhone.GetString();
		student.father.cellPhone = contacts.fatherCellPhone.GetString();
		student.father.emailAddress = contacts.fatherEmail.GetString();

		student.self.address = demographics.address.GetString();
		student.self.city = demographics.city.GetString();
		student.self.state = demographics.state.GetString();
		student.self.zipCode = demographics.zipCode.GetString();
		student.self.mailingAddress = demographics.mailingAddress.GetString();
		student.self.mailingCity = demographics.mailingCity.GetString();
		student.self.mailingState = demographics.mailingState.GetString();
		student.self.mailingZipCode = demographics.mailingZipCode.GetString();
		student.self.homePhone = demographics.homePhone.GetString();

		if ( !StoreYearData( false ) ) {
			return false;
		}

		if ( ssn.GetOldText() == "" ) {
			// we need to create the student's record
			if ( storage.StudentExists( ssn.GetString() ) ) {
				MessageBox( "A student with this SSN already exists.", "StudentInfo" );
				return false;
			} else {
				if ( !storage.CreateStudent( ssn.GetString(), schoolYear.GetSelectedIndex() ) ) {
					return false;
				}
			}
		} else {
			if ( ssn.GetOldText() != ssn.GetString() ) {
				if ( storage.StudentExists( ssn.GetString() ) ) {
					MessageBox( "A student with this SSN already exists.", "StudentInfo" );
					return false;
				} else {
					if ( !storage.ChangeSSN( ssn.GetOldText(), ssn.GetString() ) ) {
						return false;
					}
				}
			}
		}

		if ( !storage.UpdateStudent( student ) ) {
			return false;
		}
	//} else {
		//MessageBox( "The student's data has not been modified and will not be saved." );
		//MessageBox( "The student was successfully saved." );
	//	return true;
	//}
	ResetModified();
	chooser->RefreshStudentList();
	return true;
}

void CStudentEditDlg::SetReadOnly( BOOL bReadOnly, CWnd* parent ) {
	CWnd* control;
	if ( parent ) {
		control = parent->GetWindow( GW_CHILD );
	} else {
		control = GetWindow( GW_CHILD );
	}

	int yearID = schoolYear.GetSelectedIndex();

	while ( control ) {
		if ( control->IsKindOf( RUNTIME_CLASS( CDialog ) ) ) {
			SetReadOnly( bReadOnly, control );
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CStringEdit ) ) ) {
			((CStringEdit*)control)->SetReadOnly( bReadOnly );
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CIndexedListBox ) ) ) {
			((CIndexedListBox*)control)->SetReadOnly( bReadOnly );
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CGridControl ) ) ) {
			((CGridControl*)control)->SetReadOnly( bReadOnly );
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CAssessmentModsButton ) ) ) {
			((CAssessmentModsButton*)control)->SetReadOnly( bReadOnly );
		} else {
			if ( control->IsKindOf( RUNTIME_CLASS( CButton ) ) ) {
				((CButton*)control)->EnableWindow( !bReadOnly );
			}
		}
		control = control->GetWindow( GW_HWNDNEXT );
	}

	schoolYear.SetReadOnly( FALSE );
	schoolYear.Select( yearID );
	referrals.addEvaluationButton.EnableWindow( !bReadOnly );
	referrals.removeEvaluationButton.EnableWindow( !bReadOnly );
}

void CStudentEditDlg::Modify() {
	modified = true;
}

bool CStudentEditDlg::IsModified( CWnd* parent ) {
	if ( !user.CanWrite() ) {
		return false;
	}
	if ( modified ) {
		return true;
	}

	CWnd* control;
	if ( parent ) {
		control = parent->GetWindow( GW_CHILD );
	} else {
		control = GetWindow( GW_CHILD );
	}

	bool changed = false;

	while ( control && !changed ) {
		if ( control->IsKindOf( RUNTIME_CLASS( CDialog ) ) ) {
			if ( IsModified( control ) ) {
				changed = true;
				break;
				//return true;
			}
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CStringEdit ) ) ) {
			if ( ((CStringEdit*)control)->IsModified() ) {
				changed = true;
				break;
			}
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CIndexedListBox ) ) ) {
			if ( ((CIndexedListBox*)control)->IsModified() ) {
				if ( control != &referrals.evaluationRequestList ) {
					changed = true;
					break;
				}
			}
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CFancyCheckBox ) ) ) {
			if ( ((CFancyCheckBox*)control)->IsModified() ) {
				changed = true;
				break;
			}
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CAssessmentModsButton ) ) ) {
			if ( ((CAssessmentModsButton*)control)->IsModified() ) {
				changed = true;
				break;
			}
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CGridControl ) ) ) {
			//((CGridControl*)control)->EndEdit();
			if ( ((CGridControl*)control)->IsModified() ) {
				changed = true;
				break;
			}
		}
		control = control->GetWindow( GW_HWNDNEXT );
	}
	if ( !parent ) {
		string title;
		if ( ssn.GetString() == "" ) {
			title = "<No SSN>";
		} else {
			title = ssn.GetString();
		}
		if ( changed ) {
			title += " *";
		}
		CString oldTitle;
		this->GetParent()->GetWindowText( oldTitle );
		if ( strcmp( (LPCTSTR)oldTitle, title.c_str() ) ) {
			this->GetParent()->SetWindowText( title.c_str() );
		}
	}
	return changed;
}

void CStudentEditDlg::ResetModified( CWnd* parent ) {
	modified = false;

	CWnd* control;
	if ( parent ) {
		control = parent->GetWindow( GW_CHILD );
	} else {
		control = GetWindow( GW_CHILD );
	}

	while ( control ) {
		if ( control->IsKindOf( RUNTIME_CLASS( CDialog ) ) ) {
			ResetModified( control );
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CStringEdit ) ) ) {
			((CStringEdit*)control)->ResetModified();
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CIndexedListBox ) ) ) {
			((CIndexedListBox*)control)->ResetModified();
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CFancyCheckBox ) ) ) {
			((CFancyCheckBox*)control)->ResetModified();
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CAssessmentModsButton ) ) ) {
			((CAssessmentModsButton*)control)->ResetModified();
		}
		if ( control->IsKindOf( RUNTIME_CLASS( CGridControl ) ) ) {
			((CGridControl*)control)->EndEdit();
			((CGridControl*)control)->ResetModified();
		}
		control = control->GetWindow( GW_HWNDNEXT );
	}
}

bool CStudentEditDlg::IsYearDataModified() {
	return ( IsModified( &demographics  ) ||
		     IsModified( &evaluations   ) ||
			 IsModified( &assessments   ) ||
			 IsModified( &referrals     ) ||
			 IsModified( &services      ) ||
			 IsModified( &distributions ) );
}

string CStudentEditDlg::GetOldSSN() {
	return ssn.GetOldText();
}

IMPLEMENT_DYNAMIC( CStudentEditDlg, CDialog )

BEGIN_MESSAGE_MAP( CStudentEditDlg, CDialog )
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_CBN_SELCHANGE( IDC_SCHOOLYEAR, OnSchoolYearChange )
END_MESSAGE_MAP()

