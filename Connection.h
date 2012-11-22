
/*

  Remember to move the MakeString functions here.

*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "Table.h"

#pragma warning(disable:4146)
#pragma warning(disable:4786)
#include <comdef.h>
#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )

#include <vector>
#include "Variant.h"
using namespace std;

class CConnection {
	public:
		CConnection();
		~CConnection();

		bool Open(string dsn, string uid, string pass);
		void Close();

		CTable Execute( string sql );

		void BeginTransaction();
		void CommitTransaction();
		void CancelTransaction();

		bool IsOpen();

		//static string MakeString( string data );
		//static string MakeString( int data );
		//static string MakeString( CDate data );
		//static string MakeString( bool data );

		//static string MakeTableName( string name );
	protected:
		bool open;
		_ConnectionPtr conn;
};

#endif