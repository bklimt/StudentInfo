
#include "IndexedList.h"

CIndexedList::CIndexedList() {
	name = "";
}

CIndexedList::CIndexedList( string newname ) {
	name = newname;
}

CIndexedList::CIndexedList( const CIndexedList& otherList ) {
	name = otherList.name;
	data = otherList.data;
}

CIndexedList CIndexedList::operator=( const CIndexedList& otherList ) {
	name = otherList.name;
	data = otherList.data;
	return *this;
}

void CIndexedList::Clear() {
	data.clear();
}

void CIndexedList::AddString( int key, string value ) {
	data.insert( pair< int, string >( key, value ) );
}

int CIndexedList::operator[]( const char* str ) {
	map< int, string >::iterator it;
	for ( it = data.begin(); it != data.end(); it++ ) {
		if ( (string)str == (*it).second ) {
			return (*it).first;
		}
	}
	return -1;
}

string& CIndexedList::operator[]( int key ) {
	blank = "";
	if ( key == -1 ) {
		return blank;
	} else {
		if ( data.find( key ) != data.end() ) {
			return data[key];
		} else {
			return blank;
		}
	}
}

string CIndexedList::GetName() {
	return name;
}

void CIndexedList::Rewind() {
	dataIterator = data.begin();	
}

bool CIndexedList::HasMoreStrings() {
	return dataIterator != data.end();
}

string CIndexedList::GetNextString() {
	string ans;
	ans = (*dataIterator).second;
	dataIterator++;
	return ans;
}
