
/*
 * Student.h
 *
 * This class contains the data for a student's record
 *
 */

// this will keep the file from being included twice
#ifndef STUDENT_H
#define STUDENT_H

#include <string>  
#include <vector> // for the list of records for each year
using namespace std;

#include "Person.h"      // the people this student is related to, including himself
#include "StudentYear.h" // the record for this student for a single year

class CStudent {
	public:
		// constructors, copy constructors
		CStudent();
		CStudent( int schoolYearID );
		CStudent( const CStudent& otherStudent );
		CStudent operator=( const CStudent& otherStudent );

		// social security number
		string ssn;

		// the people this student is related to
		CPerson self;
		CPerson mother;
		CPerson father;

		// the set of yearly records
		vector< CStudentYear > years;
};

#endif

