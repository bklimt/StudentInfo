
#include "StudentYear.h"

CStudentYear::CStudentYear() {
	status = -1;
	eligibility = -1;
	gender = -1;
	ethnicity = -1;
	campus = -1;
	grade = -1;
	primaryDisability = -1;
	secondaryDisability = -1;
	tertiaryDisability = -1;
	instructionalSetting = -1;
	referringCampus = -1;
	referralEligible = -1;

	multipleDisabilities = false;
	medicallyFragile = false;
	medicaid = false;
}

CStudentYear::CStudentYear( const CStudentYear& otherStudentYear ) {
	CopyData( otherStudentYear );
}

CStudentYear CStudentYear::operator=( const CStudentYear& otherStudentYear ) {
	CopyData( otherStudentYear );
	return *this;
}

void CStudentYear::CopyData( const CStudentYear& otherStudentYear ) {
	schoolYearID = otherStudentYear.schoolYearID;
	birthDate = otherStudentYear.birthDate;
	statusDate = otherStudentYear.statusDate;
	lastARDDate = otherStudentYear.lastARDDate;
	lastAnnualReviewDate = otherStudentYear.lastAnnualReviewDate;
	itpDate = otherStudentYear.itpDate;
	iepStartDate = otherStudentYear.iepStartDate;
	iepEndDate = otherStudentYear.iepEndDate;
	bipDate = otherStudentYear.bipDate;
	referralDate = otherStudentYear.referralDate;
	evaluationCompletedDate = otherStudentYear.evaluationCompletedDate;
	ardCompletedDate = otherStudentYear.ardCompletedDate;
	status = otherStudentYear.status;
	eligibility = otherStudentYear.eligibility;
	gender = otherStudentYear.gender;
	ethnicity = otherStudentYear.ethnicity;
	campus = otherStudentYear.campus;
	grade = otherStudentYear.grade;
	primaryDisability = otherStudentYear.primaryDisability;
	secondaryDisability = otherStudentYear.secondaryDisability;
	tertiaryDisability = otherStudentYear.tertiaryDisability;
	instructionalSetting = otherStudentYear.instructionalSetting;
	referringCampus = otherStudentYear.referringCampus;
	referralEligible = otherStudentYear.referralEligible;
	multipleDisabilities = otherStudentYear.multipleDisabilities;
	medicallyFragile = otherStudentYear.medicallyFragile;
	demographicsComments = otherStudentYear.demographicsComments;
	contactsComments = otherStudentYear.contactsComments;
	evaluationsComments = otherStudentYear.evaluationsComments;
	servicesComments = otherStudentYear.servicesComments;
	assessmentsComments = otherStudentYear.assessmentsComments;
	referralsComments = otherStudentYear.referralsComments;
	evaluations = otherStudentYear.evaluations;
	instructionalServices = otherStudentYear.instructionalServices;
	relatedServices = otherStudentYear.relatedServices;
	esyServices = otherStudentYear.esyServices;
	evaluationsRequested = otherStudentYear.evaluationsRequested;

	k2ReadingAssessmentRecord = otherStudentYear.k2ReadingAssessmentRecord;
	readingAssessmentRecord = otherStudentYear.readingAssessmentRecord;
	mathAssessmentRecord = otherStudentYear.mathAssessmentRecord;
	writingAssessmentRecord = otherStudentYear.writingAssessmentRecord;
	scienceAssessmentRecord = otherStudentYear.scienceAssessmentRecord;
	socialStudiesAssessmentRecord = otherStudentYear.socialStudiesAssessmentRecord;
	//englishIIAssessmentRecord = otherStudentYear.englishIIAssessmentRecord;
	englishLAAssessmentRecord = otherStudentYear.englishLAAssessmentRecord;
	usHistoryAssessmentRecord = otherStudentYear.usHistoryAssessmentRecord;
	algebraIAssessmentRecord = otherStudentYear.algebraIAssessmentRecord;
	biologyAssessmentRecord = otherStudentYear.biologyAssessmentRecord;

	assessmentARDDate = otherStudentYear.assessmentARDDate;

	medicaid = otherStudentYear.medicaid;
	distributions = otherStudentYear.distributions;
	distributionsComments = otherStudentYear.distributionsComments;
}