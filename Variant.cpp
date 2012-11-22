
#include "Variant.h"
#include <afxdisp.h>

string EscapeString( string s ) {
	string ans = "";
	for ( int i=0; i<s.length(); i++ ) {
		switch ( s[i] ) {
			case '\n': ans += "\\n"; break;
			case '\r': ans += "\\r"; break;
			case '\t': ans += "\\t"; break;
			case '\'': ans += "`"; break;
			case '\\': ans += "\\\\"; break;
			default:
				ans += s[i];
		}
	}
	return ans;
}

string UnEscapeString( string s ) {
	string ans = "";
	for ( int i=0; i<s.length(); i++ ) {
		if ( s[i] == '\\' ) {
			if ( i+1 < s.length() ) {
				switch ( s[i+1] ) {
					case 'n': ans += "\n"; break;
					case 'r': ans += "\r"; break;
					case 't': ans += "\t"; break;
					case '\\': ans += "\\"; break;
					default: i--;
				}
				i++;
			}
		} else {
			ans += s[i];
		}
	}
	return ans;
}

string SerializeVariants( vector< CVariant >& vars ) {
	string ans = "";
	for ( int i=0; i<vars.size(); i++ ) {
		ans += ( vars[i].GetType() + " '*" + EscapeString( (string)vars[i] ) + "' " );
	}
	return ans;
}

vector< CVariant > UnSerializeVariants( string s ) {
	vector< CVariant > ans;

	char type[20];
	char* c = new char[s.length()+1];
	char* data = new char[s.length()+1];
	if ( !c || !data ) {
		return ans;
	}
	strcpy( c, s.c_str() );

	char* str = c;

	int pos;
	while ( c[0] ) {
		sscanf( c, "%s '%[^']' %n", type, data, &pos );
		ans.push_back( CVariant( UnEscapeString( data+1 ) ).AsType( type ) );
		c += pos;
	}

	delete[] str;
	delete[] data;

	return ans;
}

CVariant::CVariant() {
	lastCastWasValid = true;
	type = "null";
	/*	intval = -1;
	doubleval = 0;
	boolval = false;
	stringval = "";
	dateval = CDate();*/
}

void CVariant::SetNull() {
	type = "null";
	intval = -1;
	doubleval = 0;
	boolval = false;
	stringval = "";
	dateval = CDate();
}

CVariant::CVariant( const CVariant& val ) {
	lastCastWasValid = val.lastCastWasValid;
	type = val.type;
	intval = val.intval;
	doubleval = val.doubleval;
	boolval = val.boolval;
	stringval = val.stringval;
	dateval = val.dateval;
}

CVariant::CVariant( int val ) {
	lastCastWasValid = true;
	if ( val < 0 ) {
		type = "null";
	} else {
		type = "int";
		intval = val;
//		doubleval = val;
//		boolval = val ? true : false;
//		char temp[1000];
//		sprintf( temp, "%d", val );
//		stringval = temp;
//		dateval = val;
	}
}

CVariant::CVariant( double val ) {
	lastCastWasValid = true;
	type = "double";
//	intval = (int)val;
	doubleval = val;
//	boolval = ( intval ? true : false );
//	char temp[1000];
//	sprintf( temp, "%f", val );
//	stringval = temp;
//	dateval = intval;
}

CVariant::CVariant( char* val ) {
	lastCastWasValid = true;
//	string val = v;
	if ( strlen(val) == 0 ) {
		type = "null";
	} else {
		type = "string";
//		intval = -1;
//		sscanf( val.c_str(), "%d", &intval );
//		doubleval = ( intval == -1 ? 0 : intval );
//		sscanf( val.c_str(), "%f", &doubleval );
//		boolval = ( val == "T" || val == "t" || strcmpi(val.c_str(),"true")==0 );
		stringval = val;
		/*while ( stringval.size() && stringval[0] == ' ' ) {
			stringval = stringval.substr(1,stringval.size()-1);
		}
		while ( stringval.size() && stringval[stringval.size()-1] == ' ' ) {
			stringval = stringval.substr(0,stringval.size()-1);
		}*/

//		int month=0, day=0, year=0;
//		sscanf( val.c_str(), "%d-%d-%d", &month, &day, &year );
//		dateval = CDate( month, day, year );
	}
}

CVariant::CVariant( string val ) {
	lastCastWasValid = true;
	if ( val == "" ) {
		type = "null";
	} else {
		type = "string";
//		intval = -1;
//		sscanf( val.c_str(), "%d", &intval );
//		doubleval = ( intval == -1 ? 0 : intval );
//		sscanf( val.c_str(), "%f", &doubleval );
//		boolval = ( val == "T" || val == "t" || strcmpi(val.c_str(),"true")==0 );

		/*while ( val.size() && val[0] == ' ' ) {
			val = val.substr(1,val.size()-1);
		}
		while ( val.size() && val[val.size()-1] == ' ' ) {
			val = val.substr(0,val.size()-1);
		}*/
		stringval = val;

//		int month=0, day=0, year=0;
//		sscanf( val.c_str(), "%d-%d-%d", &month, &day, &year );
//		dateval = CDate( month, day, year );
	}
}

CVariant::CVariant( bool val ) {
	lastCastWasValid = true;
	type = "bool";
//	intval = ( val ? 1 : 0 );
//	doubleval = intval;
	boolval = val;
//	stringval = ( val ? "true" : "false" );
//	dateval = CDate();
}

CVariant::CVariant( const CDate& val ) {
	lastCastWasValid = true;
	if ( val == CDate() ) {
		type = "null";
	} else {
		type = "date";
//		intval = (int)val;
//		doubleval = intval;
//		boolval = false;
//		stringval = val;
		dateval = val;
	}
}

CVariant::CVariant( _variant_t val ) {
	lastCastWasValid = true;
	SetNull();
	switch (val.vt) {
		case VT_NULL : SetNull(); break;
		case VT_I4   :
		case VT_I2   : *this = (long)val; break;
		case VT_R4   : *this = (double)val; break;
		case VT_R8   : *this = (double)val; break;
		case VT_BOOL : *this = (bool)val; break;

		case VT_BSTR : {
			*this = UnEscapeString( (string)((_bstr_t)val) );
			/*
			string s = ;
			int pos = s.find( '\\' );
			while ( pos >= 0 ) {
				s = s.substr( 0, pos ) + "\r\n" + s.substr( pos+2, s.length()-(pos+2) );
				pos = s.find( '\\' );
			}
			*/
		} break;

		case VT_DATE : {
			COleDateTime dt = val;
			*this = CDate( dt.GetMonth(), dt.GetDay(), dt.GetYear() );
		} break;

		default	: SetNull(); break;
	}
}

CVariant CVariant::operator=( const CVariant& val ) {
	lastCastWasValid = val.lastCastWasValid;
	type = val.type;
	intval = val.intval;
	doubleval = val.doubleval;
	boolval = val.boolval;
	stringval = val.stringval;
	dateval = val.dateval;
	return *this;
}

bool CVariant::operator==( const CVariant& val ) {
	if ( type != val.type ) {
		return false;
	}
	if ( type == "null" ) {
		return true;
	}
	if ( type == "string" ) {
		return stringval == val.stringval;
	}
	if ( type == "int" ) {
		return intval == val.intval;
	}
	if ( type == "double" ) {
		return doubleval == val.doubleval;
	}
	if ( type == "bool" ) {
		return boolval == val.boolval;
	}
	if ( type == "date" ) {
		return dateval == val.dateval;
	}
	return true;
}

bool CVariant::operator!=( const CVariant& val ) {
	return !( (*this)==val );
}

string CVariant::GetType() {
	return type;
}

CVariant CVariant::AsType( string newType ) {
	if ( newType == "int" ) {
		int i = (int)(*this);
		if ( lastCastWasValid ) {
			return i;
		}
	}
	if ( newType == "string" ) {
		string s = (string)(*this);
		if ( lastCastWasValid ) {
			return s;
		}
	}
	if ( newType == "double" ) {
		double d = (double)(*this);
		if ( lastCastWasValid ) {
			return d;
		}
	}
	if ( newType == "bool" ) {
		bool b = (bool)(*this);
		if ( lastCastWasValid ) {
			return b;
		}
	}
	if ( newType == "date" ) {
		CDate da = (CDate)(*this);
		if ( lastCastWasValid ) {
			return da;
		}
	}
	return CVariant();
}

bool CVariant::IsNull() {
	return GetType() == "null";
}

bool CVariant::LastCastWasValid() {
	return lastCastWasValid;
}

bool CVariant::IsValidConversion( CVariant var, string type ) {
	return !var.AsType( type ).IsNull();
}

CVariant::operator int() {
	lastCastWasValid = true;
	if ( type == "int" ) {
		return intval;
	}
	if ( type == "null" ) {
		lastCastWasValid = false;
		return -1;
	}
	if ( type == "string" ) {
		float num = 0;
		int count = -1;
		sscanf( stringval.c_str(), "%f%n", &num, &count );
		if ( count != stringval.size() ) {
			lastCastWasValid = false;
			return -1;
		}
		if ( (int)num < 0 ) {
			lastCastWasValid = false;
			return -1;
		}
		return (int)num;
	}
	if ( type == "double" ) {
		return (int)doubleval;
	}
	if ( type == "bool" ) {
		return ( boolval ? 1 : 0 );
	}
	if ( type == "date" ) {
		return (int)dateval;
	}
	lastCastWasValid = false;
	return -1;
}

CVariant::operator string () {
	lastCastWasValid = true;
	if ( type == "string" ) {
		return stringval;
	}
	if ( type == "null" ) {
		lastCastWasValid = false;
		return "";
	}
	if ( type == "int" ) {
		if ( intval < 0 ) {
			lastCastWasValid = true;
			return "";
		} else {
			char temp[1000];
			sprintf( temp, "%d", intval );
			return temp;
		}
	}
	if ( type == "double" ) {
		char temp[1000];
		sprintf( temp, "%f", doubleval );
		return temp;
	}
	if ( type == "bool" ) {
		return ( boolval ? "Yes" : "No" );
	}
	if ( type == "date" ) {
		return (string)dateval;
	}
	lastCastWasValid = false;
	return "";
}

CVariant::operator double () {
	lastCastWasValid = true;
	if ( type == "double" ) {
		return doubleval;
	}
	if ( type == "null" ) {
		lastCastWasValid = false;
		return 0;
	}
	if ( type == "int" ) {
		if ( intval < 0 ) {
			lastCastWasValid = false;
			return 0;
		} else {
			return intval;
		}
	}
	if ( type == "string" ) {
		float num = 0;
		int count = -1;
		sscanf( stringval.c_str(), "%f%n", &num, &count );
		if ( count != stringval.size() ) {
			lastCastWasValid = false;
			return 0;
		}
		return num;
	}
	if ( type == "bool" ) {
		return ( boolval ? 1 : 0 );
	}
	if ( type == "date" ) {
		int num = (int)dateval;
		if ( num < 0 ) {
			lastCastWasValid = false;
			return 0;
		} else {
			return num;
		}
	}
	lastCastWasValid = false;
	return 0;
}

CVariant::operator bool () {
	lastCastWasValid = true;
	if ( type == "bool" ) {
		return boolval;
	}
	if ( type == "null" ) {
		lastCastWasValid = false;
		return false;
	}
	if ( type == "int" ) {
		return intval > 0;
	}
	if ( type == "string" ) {
		if ( stringval == "T" || stringval == "t" || stringval == "y" || stringval == "Y" || !strcmpi( stringval.c_str(), "true" ) || !strcmpi( stringval.c_str(), "yes" ) ) {
			return true;
		}
		if ( stringval == "F" || stringval == "f" || stringval == "n" || stringval == "N" || !strcmpi( stringval.c_str(), "false" ) || !strcmpi( stringval.c_str(), "no" ) ) {
			return false;
		}
		lastCastWasValid = false;
		return false;
	}
	if ( type == "double" ) {
		return doubleval != 0;
	}
	if ( type == "date" ) {
		lastCastWasValid = false;
		return false;
	}
	lastCastWasValid = false;
	return false;
}

CVariant::operator CDate() {
	lastCastWasValid = true;
	if ( type == "date" ) {
		return dateval;
	}
	if ( type == "null" ) {
		lastCastWasValid = false;
		return CDate();
	}
	if ( type == "int" ) {
		if ( intval < 0 ) {
			lastCastWasValid = false;
			return CDate();
		} else {
			return CDate(intval);
		}
	}
	if ( type == "string" ) {
		/*int month=0, day=0, year=0;
		int count = -1;
		sscanf( stringval.c_str(), "%d-%d-%d%n", &month, &day, &year, &count );
		if ( count != stringval.size() ) {
			return CDate();
		}
		return CDate( month, day, year );*/
		CDate temp = (CDate)stringval;
		if ( temp == CDate() ) {
			lastCastWasValid = false;
			return CDate();
		} else {
			return temp;
		}
	}
	if ( type == "double" ) {
		if ( (int)doubleval < 0 ) {
			lastCastWasValid = false;
			return CDate();
		} else {
			return CDate((int)doubleval);
		}
	}
	if ( type == "bool" ) {
		lastCastWasValid = false;
		return CDate();
	}
	lastCastWasValid = false;
	return CDate();
}

string CVariant::GetSQLString() {
	if ( type == "null" ) {
		return "NULL";
	}
	if ( type == "string" ) {
		string strdata = stringval;
		if ( strdata == "" ) {
			return "NULL";
		} else {
			int pos = strdata.find( '\'' );
			while ( pos >= 0 ) {
				strdata[pos] = '`';
				pos = strdata.find( '\'' );
			}
			pos = strdata.find( '\r' );
			while ( pos >= 0 ) {
				strdata = strdata.substr( 0, pos ) + "\\r" + strdata.substr( pos+1, strdata.length()-(pos+1) );
				pos = strdata.find( '\r' );
			}
			pos = strdata.find( '\n' );
			while ( pos >= 0 ) {
				strdata = strdata.substr( 0, pos ) + "\\n" + strdata.substr( pos+1, strdata.length()-(pos+1) );
				pos = strdata.find( '\n' );
			}
			return (string)"'" + strdata + "'";
		}
	}
	if ( type == "int" ) {
		if ( intval < 0 ) {
			return "NULL";
		} else {
			return (string)(*this);
		}
	}
	if ( type == "double" ) {
		return (string)(*this);
	}
	if ( type == "bool" ) {
		if ( boolval ) {
			return "'T'";
		} else {
			return "'F'";
		}
	}
	if ( type == "date" ) {
		if ( dateval == CDate() ) {
			return "NULL";
		} else {
			return (string)"'" + (string)(*this) + "'";
		}
	}
	return "NULL";
}

