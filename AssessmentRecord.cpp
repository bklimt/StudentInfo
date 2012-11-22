
#include "AssessmentRecord.h"
#include "StorageController.h"

extern CStorageController storage;

CAssessmentRecord::CAssessmentRecord() {
	achLevel = -1;
	ardExpectation = -1;
	assessment = storage.GetList("Assessment")[ "Not applicable" ];
	assessmentRecordID = -1;
	baselineAchLevel = -1;
	baselineGradeLevel = -1;
	baselineYear = -1;
	districtAssessment = "";
	gradeLevel = -1;
}

CAssessmentRecord::CAssessmentRecord( const CAssessmentRecord& otherRecord ) {
	CopyData( otherRecord );
}

CAssessmentRecord CAssessmentRecord::operator=( const CAssessmentRecord& otherRecord ) {
	CopyData( otherRecord );
	return *this;
}

void CAssessmentRecord::CopyData( const CAssessmentRecord& otherRecord ) {
	achLevel = otherRecord.achLevel;
	ardExpectation = otherRecord.ardExpectation;
	assessment = otherRecord.assessment;
	assessmentRecordID = otherRecord.assessmentRecordID;
	baselineAchLevel = otherRecord.baselineAchLevel;
	baselineGradeLevel = otherRecord.baselineGradeLevel;
	baselineYear = otherRecord.baselineYear;
	districtAssessment = otherRecord.districtAssessment;
	gradeLevel = otherRecord.gradeLevel;
	mods = otherRecord.mods;
}
