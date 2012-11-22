
#ifndef INDEXEDLIST_H
#define INDEXEDLIST_H

#pragma warning(disable:4786)

#include <string>
#include <map>
using namespace std;

class CIndexedList {
	public:
		CIndexedList();
		CIndexedList( string newname );
		CIndexedList( const CIndexedList& otherList );
		CIndexedList operator=( const CIndexedList& otherList );

		void Clear();
		void AddString( int key, string value );
		
		int operator[]( const char* str );
		string& operator[]( int key );

		string GetName();
		void Rewind();
		bool HasMoreStrings();
		string GetNextString();

	private:
		string name;
		string blank;
		map< int, string > data;
		map< int, string >::iterator dataIterator;
};

#endif
