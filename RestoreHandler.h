
#include "Connection.h"
#include <afxcmn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

class CRestoreHandler {
	public:
		CRestoreHandler();
		bool BeginRestore( string filename, CProgressCtrl* progressBar, CStatic* sql );
		bool Continue( string& message );
		void Abort();
	private:
		int current_char;
		int input;
		int line;
		int state;
		string current;
		FILE* in;
		CProgressCtrl* progress;
		CStatic* sqlBox;
		CConnection conn;
};