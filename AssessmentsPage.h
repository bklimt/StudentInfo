
#ifndef ASSESSMENTSPAGE_H
#define ASSESSMENTSPAGE_H

#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"
#include "DateEdit.h"
#include "MultiLineEdit.h"
#include "IndexedListBox.h"
#include "FancyCheckBox.h"
#include "AssessmentModsButton.h"

class CAssessmentsPage : public CDialog {
	public:
		CAssessmentsPage();
		void DoDataExchange( CDataExchange* pDX );
		BOOL PreTranslateMessage( MSG* pMsg );
		void OnOK();
		void OnCancel();
		bool OnTabPressed();
		bool OnShiftTabPressed();

/*		void OnK2ReadingModsClick();
		void OnReadingModsClick();
		void OnMathModsClick();
		void OnWritingModsClick();
		void OnScienceModsClick();
		void OnSocialStudiesModsClick();
		void OnEnglishIIModsClick();
		void OnUSHistoryModsClick();
		void OnAlgebraIModsClick();
		void OnBiologyModsClick();*/

		CIndexedListBox k2ReadingAssessment;
		//CIndexedListBox k2ReadingBaselineYear;
		//CIndexedListBox k2ReadingBaselineGrade;
		//CIndexedListBox k2ReadingBaselineAch;
		//CIndexedListBox k2ReadingARDExp;
		//CIndexedListBox k2ReadingGrade;
		//CIndexedListBox k2ReadingAch;
		CIndexedListBox readingAssessment;
		CIndexedListBox readingBaselineYear;
		CIndexedListBox readingBaselineGrade;
		CIndexedListBox readingBaselineAch;
		CIndexedListBox readingARDExp;
		CIndexedListBox readingGrade;
		CIndexedListBox readingAch;
		CIndexedListBox mathAssessment;
		CIndexedListBox mathBaselineYear;
		CIndexedListBox mathBaselineGrade;
		CIndexedListBox mathBaselineAch;
		CIndexedListBox mathARDExp;
		CIndexedListBox mathGrade;
		CIndexedListBox mathAch;
		CIndexedListBox writingAssessment;
		//CIndexedListBox writingBaselineYear;
		//CIndexedListBox writingBaselineGrade;
		//CIndexedListBox writingBaselineAch;
		CIndexedListBox writingARDExp;
		CIndexedListBox writingGrade;
		CIndexedListBox writingAch;
		CIndexedListBox scienceAssessment;
		CIndexedListBox scienceBaselineYear;
		CIndexedListBox scienceBaselineGrade;
		CIndexedListBox scienceBaselineAch;
		CIndexedListBox scienceARDExp;
		CIndexedListBox scienceGrade;
		CIndexedListBox scienceAch;
		CIndexedListBox socialStudiesAssessment;
		CIndexedListBox socialStudiesBaselineYear;
		CIndexedListBox socialStudiesBaselineGrade;
		CIndexedListBox socialStudiesBaselineAch;
		CIndexedListBox socialStudiesARDExp;
		CIndexedListBox socialStudiesGrade;
		CIndexedListBox socialStudiesAch;
		CIndexedListBox englishLAAssessment;
		CIndexedListBox englishLABaselineYear;
		CIndexedListBox englishLABaselineGrade;
		CIndexedListBox englishLABaselineAch;
		CIndexedListBox englishLAARDExp;
		CIndexedListBox englishLAGrade;
		CIndexedListBox englishLAAch;

		/*
		CIndexedListBox usHistoryAssessment;
		CFancyCheckBox usHistoryBaselineYear;
		CIndexedListBox usHistoryBaselineGrade;
		CIndexedListBox usHistoryBaselineAch;
		CFancyCheckBox usHistoryARDExp;
		CIndexedListBox usHistoryGrade;
		CIndexedListBox usHistoryAch;
		CIndexedListBox algebraIAssessment;
		CFancyCheckBox algebraIBaselineYear;
		CIndexedListBox algebraIBaselineGrade;
		CIndexedListBox algebraIBaselineAch;
		CFancyCheckBox algebraIARDExp;
		CIndexedListBox algebraIGrade;
		CIndexedListBox algebraIAch;
		CIndexedListBox biologyAssessment;
		CFancyCheckBox biologyBaselineYear;
		CIndexedListBox biologyBaselineGrade;
		CIndexedListBox biologyBaselineAch;
		CFancyCheckBox biologyARDExp;
		CIndexedListBox biologyGrade;
		CIndexedListBox biologyAch;
		*/

		CAssessmentModsButton k2ReadingMods;
		CAssessmentModsButton readingMods;
		CAssessmentModsButton mathMods;
		CAssessmentModsButton writingMods;
		CAssessmentModsButton scienceMods;
		CAssessmentModsButton socialStudiesMods;
		CAssessmentModsButton englishLAMods;
		//CAssessmentModsButton usHistoryMods;
		//CAssessmentModsButton algebraIMods;
		//CAssessmentModsButton biologyMods;

		CDateEdit assessmentARDDate;
		CMultiLineEdit assessmentsComments;
};

#endif
