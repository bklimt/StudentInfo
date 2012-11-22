
#include "Student.h"

CStudent::CStudent() {
}

CStudent::CStudent( int schoolYearID ) {
	CStudentYear year;
	year.schoolYearID = schoolYearID;
	years.push_back( year );
}

CStudent::CStudent( const CStudent& otherStudent ) {
	ssn = otherStudent.ssn;
	self = otherStudent.self;
	mother = otherStudent.mother;
	father = otherStudent.father;
	years = otherStudent.years;
}

CStudent CStudent::operator=( const CStudent& otherStudent ) {
	ssn = otherStudent.ssn;
	self = otherStudent.self;
	mother = otherStudent.mother;
	father = otherStudent.father;
	years = otherStudent.years;
	return *this;
}
