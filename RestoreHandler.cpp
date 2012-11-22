
#pragma warning(disable:4786)
#include "RestoreHandler.h"
#include "MainWindow.h"
#include "Preferences.h"
#include "User.h"
#include <afxdisp.h>
using namespace std;

#define STATE_COMMAND 0
#define STATE_SEARCH 1
#define STATE_STRING 2
#define STATE_FINISHED 3
#define STATE_ERROR 4

extern CPreferences preferences;
extern CUser user;

CRestoreHandler::CRestoreHandler() {
}

bool CRestoreHandler::BeginRestore( string filename, CProgressCtrl* progressBar, CStatic* sql ) {
	progress = progressBar;
	sqlBox = sql;
	in = fopen( filename.c_str(), "r" );
	if ( !in ) {
		return false;
	}
	if ( !conn.Open( preferences.GetString("DSN"), "StudentInfoAdmin", user.GetAdminPassword() ) ) {
		fclose(in);
		return false;
	}
	line = 1;
	state = STATE_SEARCH;
	string current = "";
	current_char = -1;
	fseek( in, 0, SEEK_END );
	progress->SetRange32( 0, ftell( in ) );
	fseek( in, 0, SEEK_SET );
	return true;
}

bool CRestoreHandler::Continue( string& message ) {
	if ( !in || state == STATE_ERROR ) {
		message = "Restore could not be completed.";
		if ( in ) fclose( in );
		conn.Close();
		return false;
	}
	
	if ( state == STATE_FINISHED ) {
		message = "Restore completed successfully.";
		fclose( in );
		conn.Close();
		return false;
	}

	current_char++;
	progress->SetPos( current_char );
	input = fgetc( in );
	switch ( input ) {
		case EOF :
			if ( state == STATE_SEARCH ) {
				state = STATE_FINISHED;
			} else {
				CString temp;
				temp.Format( "[ ERROR ] line %d : %s\n(no semi-colon?)\n", line, current.c_str() );
				//mainWnd->MessageBox( temp, "StudentInfo" );
				message = (LPCTSTR)temp;
				state = STATE_ERROR;
			}
			break;
		
		case '\n' : line++;
		case ' '  :
		case '\t' :
		case '\r' :
			if ( state == STATE_COMMAND || state == STATE_STRING ) {
				current += ' ';
			}
			break;

		case ';' :
			if ( state == STATE_COMMAND ) {
				state = STATE_SEARCH;
				try {
					sqlBox->SetWindowText( current.c_str() );
					conn.Execute( current.c_str() );
					current = "";
				}
				catch ( ... ) {
					CString temp;
					temp.Format( "[ ERROR ] line %d : %s\n", line, current.c_str() );
					//mainWnd->MessageBox( temp, "StudentInfo" );
					message = (LPCTSTR)temp;
					state = STATE_ERROR;
				}
			} else {
				if ( state == STATE_STRING ) {
					current += ';';
				}
			}
			break;

		case '\'' :
			if ( state != STATE_STRING ) {
				state = STATE_STRING;
				current += '\'';
			} else {
				state = STATE_COMMAND;
				current += '\'';
			}
			break;

		default :
			current += (char)input;
			if ( state == STATE_SEARCH ) {
				state = STATE_COMMAND;
			}
			break;
	}
	return true;
}

void CRestoreHandler::Abort() {
	fclose( in );
	conn.Close();
}
