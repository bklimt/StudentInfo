
#ifndef VARIANT_H
#define VARIANT_H

#include <afx.h>
#include <string>
#include <vector>
#include <comdef.h>
#include "Date.h"
using namespace std;

class CVariant {
	public:
		CVariant();
		CVariant( const CVariant& val );
		CVariant( int val );
		CVariant( double val );
		CVariant( char* v );
		CVariant( string val );
		CVariant( bool val );
		CVariant( const CDate& val );
		CVariant( _variant_t val );

		CVariant operator=( const CVariant& val );
		bool operator==( const CVariant& val );
		bool operator!=( const CVariant& val );

		string GetType();
		CVariant AsType( string newType );
		bool IsNull();
		bool LastCastWasValid();
		static bool IsValidConversion( CVariant var, string type );
		void SetNull();

		operator int ();
		operator string ();
		operator double ();
		operator bool ();
		operator CDate ();

		string GetSQLString();
	private:
		string type;
		int intval;
		double doubleval;
		bool boolval;
		CDate dateval;
		string stringval;
		bool lastCastWasValid;
};

string EscapeString( string s );
string UnEscapeString( string s );
string SerializeVariants( vector< CVariant >& vars );
vector< CVariant > UnSerializeVariants( string s );

#endif