
#pragma warning(disable:4786)
#include "BackupHandler.h"
#include "MainWindow.h"
#include "Preferences.h"
#include "Variant.h"
#include "User.h"
#include <afxdisp.h>
#include <iostream>
using namespace std;

extern CPreferences preferences;
extern CUser user;

string MakeBackupTableName( string name ) {
	if ( preferences.GetBool( "UsePrefix" ) ) {
		return "StudentInfoSuperUser." + name;
	} else {
		return name;
	}
}

CBackupHandler::CBackupHandler() {
	success = false;
}

bool CBackupHandler::BeginBackup( vector<string> backupTables, string filename, CProgressCtrl* progressBar ) {
	for ( int j=0; j<backupTables.size(); j++ ) {
		backupTables[j] = MakeBackupTableName( backupTables[j] );
	}
	tables = backupTables;
	progress = progressBar;
	initialized = false;
	needNewTable = true;
	currentTable = -1;
	out.open( filename.c_str() );
	for ( int i=backupTables.size()-1; i>=0; i-- ) {
		out << "DELETE FROM " << backupTables[i] << ";" << endl;
	}
	return true;
}

bool CBackupHandler::Continue( string& message ) {
	try {
		if ( currentTable >= (int)tables.size() ) {
			success = true;
			message = "Backup completed successfully.";
			return false;
		}
		if ( !initialized ) {
			Open( preferences.GetString( "DSN" ), "StudentInfoReader", user.GetReaderPassword() );
			initialized = true;
		}
		if ( needNewTable ) {
			currentTable++;
			if ( currentTable == tables.size() ) {
				out.close();
				success = true;
				message = "Backup completed successfully.";
				return false;
			}
			needNewTable = false;
			currentRecord = 0;
			_variant_t ra( 0L );
			rs = conn->Execute( ((string)"SELECT COUNT(*) FROM " + tables[currentTable]).c_str(), &ra, adOptionUnspecified );
			int count = (long)rs->GetCollect((long)0);
			rs = conn->Execute( ((string)"SELECT * FROM " + tables[currentTable]).c_str(), &ra, adOptionUnspecified );
			if ( rs->State == adStateClosed ) {
				message = "Backup failed.";
				out.close();
				return false;
			}
 			progress->SetRange32( 0, count );
			progress->SetPos( 0 );
		}
		if ( rs->GetadoEOF() ) {
			rs->Close();
			needNewTable = true;
		} else {
			progress->SetPos( currentRecord++ );
			int i;
			out << "INSERT INTO " << tables[currentTable] << "( ";
			for ( i=0; i<rs->GetFields()->GetCount(); i++ ) {
				string fieldName = (LPCTSTR)rs->GetFields()->GetItem((long)i)->Name;
				out << fieldName;
				if ( i!=rs->GetFields()->GetCount()-1) {
					out << ", ";
				}
			}
			out << " ) VALUES ( ";
			for ( i=0; i<rs->Fields->GetCount(); i++ ) {
				/*
				_variant_t val = rs->GetCollect( (long)i );
				CVariant   temp;
				switch (val.vt) {
					case VT_NULL : temp.SetNull(); break;
					case VT_I4   :
					case VT_I2   : temp = (long)val; break;
					case VT_R4   : temp = (double)val; break;
					case VT_R8   : temp = (double)val; break;
					case VT_BOOL : temp = (bool)val; break;

					case VT_BSTR : {
						string s = (string)((_bstr_t)val);
						int pos = s.find( '\\' );
						while ( pos >= 0 ) {
							s = s.substr( 0, pos ) + "\r\n" + s.substr( pos+2, s.length()-(pos+2) );
							pos = s.find( '\\' );
						}
						temp = s;
					} break;

					case VT_DATE : {
						COleDateTime dt = val;
						temp = CDate( dt.GetMonth(), dt.GetDay(), dt.GetYear() );
					} break;

					default		 : temp.SetNull(); break;
				}
				if ( temp.GetType() == "int" ) {
					out << GetSQLString( (int)temp );
				} else {
					if ( temp.GetType() == "null" ) {
						out << "NULL";
					} else {
						if ( temp.GetType() == "date" ) {
							out << GetSQLString( (CDate)temp );
						} else {
							if ( temp.GetType() == "bool" ) {
								out << GetSQLString( (bool)temp );
							} else {
								out << GetSQLString( (string)temp );
							}
						}
					}
				}
				*/
				CVariant temp = rs->GetCollect( (long)i );
				out << temp.GetSQLString();

				if ( i!=rs->GetFields()->GetCount()-1) {
					out << ", ";
				}
			}
			out << " );" << endl;
			rs->MoveNext();
		}
		return true;
	}
	catch (...) {
		out.close();
		message = "Backup failed.";
		return false;
	}
}

void CBackupHandler::Abort() {
	out.close();
	if ( initialized ) {
		Close();
	}
}

int CBackupHandler::GetCurrentTable() {
	if ( needNewTable ) {
		return currentTable + 1;
	} else {
		return currentTable;
	}
}

bool CBackupHandler::WasSuccessful() {
	return success;
}
