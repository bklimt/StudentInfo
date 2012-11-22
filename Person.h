
/*
 * Person.h
 *
 * This class contains the data for a person's record
 *
 */

// keep the file from being included twice
#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class CPerson {
	public:
		// constructors
		CPerson();
		CPerson( const CPerson& otherPerson );
		CPerson operator=( const CPerson& otherPerson );
		void CopyData( const CPerson& otherPerson );

		// "person"-al information
		int personID;
		string firstName;
		string middleName;
		string lastName;
		string address;
		string city;
		string state;
		string zipCode;
		string mailingAddress;
		string mailingCity;
		string mailingState;
		string mailingZipCode;
		string workPhone;
		string homePhone;
		string cellPhone;
		string emailAddress;
};

#endif
