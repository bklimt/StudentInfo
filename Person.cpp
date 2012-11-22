
#include "Person.h"

CPerson::CPerson() {
	state = "TX";
	mailingState = "TX";
	//workPhone = "254-";
	//homePhone = "254-";
	//cellPhone = "254-";
}

CPerson::CPerson( const CPerson& otherPerson ) {
	CopyData( otherPerson );
}

CPerson CPerson::operator=( const CPerson& otherPerson ) {
	CopyData( otherPerson );
	return *this;
}

void CPerson::CopyData( const CPerson& otherPerson ) {
	personID = otherPerson.personID;
	firstName = otherPerson.firstName;
	middleName = otherPerson.middleName;
	lastName = otherPerson.lastName;
	address = otherPerson.address;
	city = otherPerson.city;
	state = otherPerson.state;
	zipCode = otherPerson.zipCode;
	workPhone = otherPerson.workPhone;
	homePhone = otherPerson.homePhone;
	cellPhone = otherPerson.cellPhone;
	emailAddress = otherPerson.emailAddress;
	mailingAddress = otherPerson.mailingAddress;
	mailingCity = otherPerson.mailingCity;
	mailingState = otherPerson.mailingState;
	mailingZipCode = otherPerson.mailingZipCode;
}
