
#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#pragma warning(disable:4786)

#include "Connection.h"
#include "User.h"
#include "Student.h"
#include "IndexedList.h"
//#include <afxcmn.h>
//#include <iostream>
using namespace std;

class CStorageController {
	public:
		CStorageController();
		~CStorageController();

		CIndexedList GetSchoolYearList();

		vector<string> GetUserList();
		CUser GetUser( string username, string adminPassword="" );

		bool ValidateUser( string username, string password );

		bool CreateUser( string username );
		bool ChangeAdminPassword( string newPassword );
		bool UpdateUser( string username, CUser newUser, string adminPassword="" );
		bool DeleteUser( string username );

		bool GetStudentList( CTable& table, string first, string last, string ssn, int year );
		void GetAssessmentRecord( CConnection* conn, CAssessmentRecord& record, int recordID );
		bool GetStudent( CStudent& student, string ssn, int year, CConnection* conn=NULL );
		CIndexedList GetList( string listName, CConnection* conn=NULL );

		bool StudentExists( string ssn );
		bool CreateStudent( string ssn, int schoolYearID );
		bool ChangeSSN( string oldSSN, string newSSN );
		void UpdatePerson( string personID, CPerson& person, CConnection& conn );
		void UpdateAssessmentRecord( CConnection* conn, CAssessmentRecord& record );
		bool UpdateStudent( CStudent& student );

		bool DeleteStudentYear( string ssn, int schoolYearID );

		bool RemoveListItem( string listName, int index );
		bool AddListItem( string listName, int index, string value );
		bool ModifyListItem( string listName, int index, string value );

		bool OpenForAuth( CConnection* conn );
		bool OpenForReading( CConnection* conn );
		bool OpenForWriting( CConnection* conn );
		bool OpenForAdmin( CConnection* conn );
		//void BeginBackup( vector<string> tables, ostream& newOut, CProgressCtrl* newProgress );
		//bool ContinueBackup();

		bool SafeAuthQuery( CTable& table, string sql, string error, CConnection* conn=NULL );
		bool SafeQuery( CTable& table, string sql, string error, CConnection* conn=NULL );
		bool SafeUpdate( string sql, string error );
		bool SafeAdminUpdate( string sql, string error );

	private:
		// backup data
		/*vector<string> backupTables;
		bool needNewTable;
		int currentTable;
		int currentRow;
		CProgressCtrl* progress;
		CTable currentRS;
		ostream& backupOut;*/
};

// with the preprocessor directives below, any file which includes this one has access
//   to the global "storage" object, except the file which instantiates the object by
//   defining DECLARE_STORAGE
//#ifdef DECLARE_STORAGE
//	CStorageController storage;
//#else
//	extern CStorageController storage;
//#endif
//
#endif