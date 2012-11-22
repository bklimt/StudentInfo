
/*
 * StudentYear.h
 *
 * This class contains the data for a student's yearly record
 *
 */

// this will keep the file from being included twice
#ifndef STUDENTYEAR_H
#define STUDENTYEAR_H

#include <string>
#include <vector>
using namespace std;

// include the datatypes used in this record
#include "Date.h"
#include "Evaluation.h"
#include "Service.h"
#include "AssessmentRecord.h"
#include "Distribution.h"

class CStudentYear {
	public:
		// constructors
		CStudentYear();
		CStudentYear( const CStudentYear& otherStudentYear );
		CStudentYear operator=( const CStudentYear& otherStudentYear );
		void CopyData( const CStudentYear& otherStudentYear );

		// which school year this record is for
		int schoolYearID;

		// the data that must be stored in the record
		CDate birthDate;
		CDate statusDate;
		CDate lastARDDate;
		CDate lastAnnualReviewDate;
		CDate itpDate;
		CDate iepStartDate;
		CDate iepEndDate;
		CDate bipDate;
		CDate referralDate;
		CDate evaluationCompletedDate;
		CDate ardCompletedDate;

		int status;
		int eligibility;
		int gender;
		int ethnicity;
		int campus;
		int grade;
		int primaryDisability;
		int secondaryDisability;
		int tertiaryDisability;
		int instructionalSetting;
		int referringCampus;
		int referralEligible;

		bool multipleDisabilities;
		bool medicallyFragile;
		bool medicaid;

		string demographicsComments;
		string contactsComments;
		string evaluationsComments;
		string servicesComments;
		string assessmentsComments;
		string referralsComments;
		string distributionsComments;

		vector< CDistribution > distributions;
		vector< CEvaluation > evaluations;
		vector< CService > instructionalServices;
		vector< CService > relatedServices;
		vector< CService > esyServices;
		vector< int > evaluationsRequested;

		CAssessmentRecord k2ReadingAssessmentRecord;
		CAssessmentRecord readingAssessmentRecord;
		CAssessmentRecord mathAssessmentRecord;
		CAssessmentRecord writingAssessmentRecord;
		CAssessmentRecord scienceAssessmentRecord;
		CAssessmentRecord socialStudiesAssessmentRecord;
		//CAssessmentRecord englishIIAssessmentRecord; no longer used by state
		CAssessmentRecord englishLAAssessmentRecord;
		CAssessmentRecord usHistoryAssessmentRecord;
		CAssessmentRecord algebraIAssessmentRecord;
		CAssessmentRecord biologyAssessmentRecord;

		CDate assessmentARDDate;
};

#endif

