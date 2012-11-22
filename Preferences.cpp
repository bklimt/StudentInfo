
#include "Preferences.h"
#include "XMLObject.h"

void CPreferences::UseDefaults() {
	stringMap["DSN"] = "Driver={SQL Server};Server=misd-sqlserver;Address=misd-sqlserver,1433;Network=DBMSSOCN;Database=StudentInfo2;";

	rectMap["MainWindowRect"] = CRect( 0, 0, 800, 600 );
	rectMap["StudentChooserRect"] = CRect( 0, 0, 250, 500 );

	boolMap["ShowNewStudentOptions"] = true;
	boolMap["ShowStudentChooser"] = true;

	intMap["MainWindowState"] = SW_NORMAL;
	intMap["ChooserWindowState"] = SW_NORMAL;
	intMap["CurrentSchoolYear"] = 1;

	stringMap["ConnectionType"] = "SQL";
	stringMap["DSNDatabase"] = "StudentInfo";
	stringMap["AccessDatabase"] = "studentinfo.mdb";
	stringMap["SQLServer"] = "misd-sqlserver";
	stringMap["SQLAddress"] = "misd-sqlserver,1433";
	stringMap["SQLDatabase"] = "StudentInfo2";

	boolMap["UsePrefix"] = true;
}

CPreferences::CPreferences() {
	iniPath = "";
	UseDefaults();
	//char temp[10000];
	//::GetCurrentDirectory( 9999, temp );
	//iniPath = temp;
	//if ( iniPath[iniPath.size()-1] != '\\' ) {
	//	iniPath += '\\';
	//}
	//LoadPreferences();
}

CPreferences::~CPreferences() {
	SavePreferences();
}

void CPreferences::LoadPreferences() {
	UseDefaults();
	try {
		CXMLObject obj;
		if ( !obj.LoadFromFile( GetHomePath()+"studentinfo.prefs" ) ) {
			return;
		}
		if ( obj.GetType() != "PREFERENCES" ) {
			return;
		}
		for ( int i=0; i<obj.GetChildCount(); i++ ) {
			if ( obj.GetChild(i)->GetType() == "STRING" ) {
				stringMap[ obj.GetChild(i)->GetParameter("NAME") ] = obj.GetChild(i)->GetParameter("VALUE");
			} else {
				if ( obj.GetChild(i)->GetType() == "RECT" ) {
					int top,left,right,bottom;
					sscanf( obj.GetChild(i)->GetParameter("TOP").c_str(), "%d", &top );
					sscanf( obj.GetChild(i)->GetParameter("LEFT").c_str(), "%d", &left );
					sscanf( obj.GetChild(i)->GetParameter("RIGHT").c_str(), "%d", &right );
					sscanf( obj.GetChild(i)->GetParameter("BOTTOM").c_str(), "%d", &bottom );
					rectMap[ obj.GetChild(i)->GetParameter("NAME") ] = CRect( left, top, right, bottom );
				} else {
					if ( obj.GetChild(i)->GetType() == "BOOL" ) {
						boolMap[ obj.GetChild(i)->GetParameter("NAME") ] = ( obj.GetChild(i)->GetParameter("VALUE")=="true" ? true : false );
					} else {
						if ( obj.GetChild(i)->GetType() == "INT" ) {
							int temp;
							sscanf( obj.GetChild(i)->GetParameter("VALUE").c_str(), "%d", &temp );
							intMap[ obj.GetChild(i)->GetParameter("NAME") ] = temp;
						}
					}
				}
			}
		}
		/*ifstream in( ( iniPath + "querywizard2.ini" ).c_str() );
		string name = "";
		string type = "";
		string value = "";
		while ( type != "end" ) {
			if ( !in.good() ) {
				UseDefaults();
				return;
			}
			getline( in, name );
			getline( in, type );
			if ( type == "string" ) {
				getline( in, value );
				for ( int i=0; i<value.size()-1; i++ ) {
					if ( value[i] == '\\' && value[i+1] == 'n' ) {
						value = value.substr( 0,i ) + "\n" + value.substr( i+2, value.length()-(i+2) );
					}
				}
				stringMap[name] = value;
			} else {
				if ( type == "rect" ) {
					CRect temp;
					getline( in, value );
					sscanf( value.c_str(), "%d", &temp.bottom );
					getline( in, value );
					sscanf( value.c_str(), "%d", &temp.left );
					getline( in, value );
					sscanf( value.c_str(), "%d", &temp.right );
					getline( in, value );
					sscanf( value.c_str(), "%d", &temp.top );
					rectMap[name] = temp;
				} else {
					if ( type == "bool" ) {
						getline( in, value );
						if ( value == "true" ) {
							boolMap[name] = true;
						} else {
							boolMap[name] = false;
						}
					} else {
						if ( type == "int" ) {
							int temp;
							getline( in, value );
							sscanf( value.c_str(), "%d", &temp );
							intMap[name] = temp;
						}
					}
				}
			}
			if ( type != "end" ) {
				getline( in, value );
			}
		}
		in.close();
		*/
	}
	catch ( ... ) {
		UseDefaults();
	}
}

void CPreferences::SavePreferences() {
	try {
		//FILE* f = fopen(  ( iniPath + "querywizard2.ini" ).c_str(), "w" );
		CXMLObject obj;
		obj.SetType( "PREFERENCES" );
		map<string,string>::iterator strIt;
		map<string,CRect>::iterator rectIt;
		map<string,bool>::iterator boolIt;
		map<string,int>::iterator intIt;

		for ( strIt = stringMap.begin(); strIt != stringMap.end(); strIt++ ) {
			CXMLObject child;
			child.SetType( "STRING" );
			child.AddParameter( "NAME", (*strIt).first );
			child.AddParameter( "VALUE", (*strIt).second );
			obj.AddChild( child );
			//fprintf( f, "%s\n", (*strIt).first.c_str() );
			//fprintf( f, "string\n" );
			//string value = (*strIt).second;
			//while ( value.find( '\n' ) != -1 ) {
			//	int pos = value.find( '\n' );
			//	value = value.substr(0,pos) + "\\n" + value.substr(pos+1,value.size()-(pos+1));
			//}
			//fprintf( f, "%s\n\n", value.c_str() );
		}

		for ( rectIt = rectMap.begin(); rectIt != rectMap.end(); rectIt++ ) {
			char temp[1000];
			CXMLObject child;
			child.SetType( "RECT" );
			child.AddParameter( "NAME", (*rectIt).first );
			sprintf( temp, "%d", (*rectIt).second.top );
			child.AddParameter( "TOP", temp );
			sprintf( temp, "%d", (*rectIt).second.left );
			child.AddParameter( "LEFT", temp );
			sprintf( temp, "%d", (*rectIt).second.right );
			child.AddParameter( "RIGHT", temp );
			sprintf( temp, "%d", (*rectIt).second.bottom );
			child.AddParameter( "BOTTOM", temp );
			obj.AddChild( child );
			//fprintf( f, "%s\n", (*rectIt).first.c_str() );
			//fprintf( f, "rect\n" );
			//CRect value = (*rectIt).second;
			//fprintf( f, "%d\n", value.bottom );
			//fprintf( f, "%d\n", value.left );
			//fprintf( f, "%d\n", value.right );
			//fprintf( f, "%d\n\n", value.top );
		}

		for ( boolIt = boolMap.begin(); boolIt != boolMap.end(); boolIt++ ) {
			CXMLObject child;
			child.SetType( "BOOL" );
			child.AddParameter( "NAME", (*boolIt).first );
			child.AddParameter( "VALUE", (*boolIt).second ? "true" : "false" );
			obj.AddChild( child );
			//fprintf( f, "%s\n", (*boolIt).first.c_str() );
			//fprintf( f, "bool\n" );
			//if ( (*boolIt).second ) {
			//	fprintf( f, "true\n\n" );
			//} else {
			//	fprintf( f, "false\n\n" );
			//}
		}

		for ( intIt = intMap.begin(); intIt != intMap.end(); intIt++ ) {
			char temp[1000];
			CXMLObject child;
			child.SetType( "INT" );
			child.AddParameter( "NAME", (*intIt).first );
			sprintf( temp, "%d", (*intIt).second );
			child.AddParameter( "VALUE", temp );
			obj.AddChild( child );
			//fprintf( f, "%s\n", (*intIt).first.c_str() );
			//fprintf( f, "int\n" );
			//fprintf( f, "%d\n\n", (*intIt).second );
		}

		//fprintf( f, "end\nend\nend\n" );
		//fclose(f);
		obj.SaveToFile( GetHomePath()+"studentinfo.prefs" );
	}
	catch ( ... ) {
	}
}

string CPreferences::GetHomePath() {
	return iniPath;
}

void CPreferences::SetHomePath( string newPath ) {
	iniPath = newPath;
}

string CPreferences::GetString( string name ) {
	if ( stringMap.find( name ) == stringMap.end() ) {
		return "";
	}
	return stringMap[name];
}

CRect CPreferences::GetRect( string name ) {
	if ( rectMap.find( name ) == rectMap.end() ) {
		return CRect(0,0,0,0);
	}
	return rectMap[name];
}

bool CPreferences::GetBool( string name ) {
	if ( boolMap.find( name ) == boolMap.end() ) {
		return false;
	}
	return boolMap[name];
}

int CPreferences::GetInt( string name ) {
	if ( intMap.find( name ) == intMap.end() ) {
		return 0;
	}
	return intMap[name];
}

void CPreferences::SetString( string name, string value ) {
	stringMap[name] = value;
}

void CPreferences::SetRect( string name, CRect value ) {
	rectMap[name] = value;
}

void CPreferences::SetBool( string name, bool value ) {
	boolMap[name] = value;
}

void CPreferences::SetInt( string name, int value ) {
	intMap[name] = value;
}
