
#include "Connection.h"
#include <afxcmn.h>
#include <fstream>
using namespace std;

class CBackupHandler : public CConnection {
	public:
		CBackupHandler();
		bool BeginBackup( vector<string> backupTables, string filename, CProgressCtrl* progressBar );
		bool Continue( string& message );
		void Abort();
		int GetCurrentTable();
		bool WasSuccessful();
	private:
		bool needNewTable;
		bool initialized;
		int currentTable;
		int currentRecord;
		bool success;
		ofstream out;
		vector<string> tables;
		CProgressCtrl* progress;
		_RecordsetPtr rs;
};