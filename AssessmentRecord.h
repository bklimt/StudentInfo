
/*
 * AssessmentRecord.h
 *
 * This class contains the data for an assessment of a student
 *
 */

// keep the file from being included twice
#ifndef ASSESSMENTRECORD_H
#define ASSESSMENTRECORD_H

#include <string>
#include <vector>
using namespace std;

class CAssessmentRecord {
	public:
		// constructors, copy constructors
		CAssessmentRecord();
		CAssessmentRecord( const CAssessmentRecord& otherRecord );
		CAssessmentRecord operator=( const CAssessmentRecord& otherRecord );
		void CopyData( const CAssessmentRecord& otherRecord );

		// This is the data that must be kept for an assessment.
		//   As with the other data, integers are indices into indexed lists
		//   that are maintained by the database

		int assessmentRecordID;
		int assessment;
		int baselineYear;
		int baselineGradeLevel;
		int baselineAchLevel;
		int ardExpectation;
		int gradeLevel;
		int achLevel;
		string districtAssessment;

		// the set of mods that are associated with this assessment
		vector<int> mods;
};

#endif
