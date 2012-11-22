
#include "Table.h"

CTable::CTable() { record = 0; }

CTable::CTable( const CTable& table ) {
	data = table.data;
	columns = table.columns;
	record = table.record;
	columnMap = table.columnMap;
}

CTable CTable::operator=( const CTable& table ) {
	data = table.data;
	columns = table.columns;
	record = table.record;
	columnMap = table.columnMap;
	return *this;
}

void CTable::SetFieldNames( vector<string> names ) {
	columns = names;
	columnMap.clear();
	for ( int i=0; i<columns.size(); i++ ) {
		CString temp = columns[i].c_str();
		temp.MakeUpper();
		columnMap[ (string)(LPCTSTR)temp ] = i;
	}
}

void CTable::SetData( vector< vector< CVariant > > newdata ) {
	data = newdata;
}

vector<string> CTable::GetFieldNames() {
	return columns;
}

int CTable::GetColumnCount() {
	return columns.size();
}

int CTable::GetRowCount() {
	return data.size();
}

void CTable::MoveTo( int index ) {
	record = index;
}

void CTable::MoveNext() {
	record++;
}

bool CTable::IsAtEOF() {
	return ( record >= data.size() );
}

bool CTable::IsEmpty() {
	return ( data.size() == 0 );
}

CVariant CTable::operator[]( char* index ) {
	CString idx = index;
	idx.MakeUpper();
	if ( columnMap.find( (string)(LPCTSTR)idx ) != columnMap.end() ) {
		return (*this)[ columnMap[ (string)(LPCTSTR)idx ] ];
	}
/*
	for ( int i=0; i<columns.size(); i++ ) {
		CString s1 = index;
		CString s2 = columns[i].c_str();
		s1.MakeUpper();
		s2.MakeUpper();
		if ( s1 == s2 ) {
			return (*this)[i];
		}
	}
*/
	char temp[1000];
	sprintf( temp, "bad: %s", index );
	MessageBox( 0, temp, "", 0 );
	return CVariant();
}

CVariant CTable::operator[]( int index ) {
	if ( record >= 0 &&
		 record <  data.size() &&
		 index  >= 0 && 
		 index  <  data[record].size() ) {
		return data[record][index];
	} else {
		return CVariant();
	}
}
