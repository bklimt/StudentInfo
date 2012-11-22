
#ifndef STUDENTEDITDLG_H
#define STUDENTEDITDLG_H

#pragma warning(disable:4786)
#include <afxwin.h>
#include <afxdlgs.h>
#include <vector>
using namespace std;

#include "DemographicsPage.h"
#include "ContactsPage.h"
#include "EvaluationsPage.h"
#include "ServicesPage.h"
#include "AssessmentsPage.h"
#include "ReferralsPage.h"
#include "DistributionsPage.h"

#include "resource.h"
#include "Student.h"
#include "IndexedList.h"
#include "StringEdit.h"
#include "SSNEdit.h"
#include "TabButton.h"
#include "User.h"

class CStudentEditDlg : public CDialog {
	public:
		void DoDataExchange( CDataExchange* pDX );
		BOOL PreTranslateMessage( MSG* pMsg );
		int  OnCreate( LPCREATESTRUCT lpCreateStruct );
		void OnSetFocus( CWnd* pOldWnd );
		bool AttemptClose();
		void OnSchoolYearChange();
		void OnOK();
		void OnCancel();
		void OnTabPressed();
		//void OnCancel();
		//void OnDemographicTabClicked();
		//void OnContactTabClicked();

		void ShowNextPage( CDialog* page );
		void ShowPreviousPage( CDialog* page );
		void ShowStudent( CStudent& studentData, int yearID );

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
		);
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
		);
		bool ValidateStudent();
		void LoadYearData( int yearID );
		bool StoreYearData( bool checkModified=true );
		bool SaveStudent();
		void SetReadOnly( BOOL bReadOnly=TRUE, CWnd* parent=NULL );

		void Modify();
		bool IsModified( CWnd* parent=NULL );
		void ResetModified( CWnd* parent=NULL );
		bool IsYearDataModified();

		string GetOldSSN();

	private:
		//bool   isNew;
		//string oldSSN;
		int currentYearID;

		CSSNEdit    ssn;
		CStringEdit firstName;
		CStringEdit middleName;
		CStringEdit lastName;
		CIndexedListBox schoolYear;

		CTabButton demographicsTab;
		CTabButton contactsTab;
		CTabButton evaluationsTab;
		CTabButton assessmentsTab;
		CTabButton servicesTab;
		CTabButton referralsTab;
		CTabButton distributionsTab;

		CDemographicsPage demographics;
		CContactsPage contacts;
		CEvaluationsPage evaluations;
		CServicesPage services;
		CAssessmentsPage assessments;
		CReferralsPage referrals;
		CDistributionsPage distributions;

		vector< CTabButton* > tabs;
		vector< CDialog* > pages;

		CStudent student;
		bool modified;

		DECLARE_DYNAMIC( CStudentEditDlg )
		DECLARE_MESSAGE_MAP()
};

#endif
