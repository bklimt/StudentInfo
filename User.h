
/*
 * User.h
 *
 * This class contains the data for a user of the program
 *
 */

// keep the file from being included twice
#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class CUser {
	public:
		// constructors
		CUser();
		CUser( const CUser& user );
		CUser operator=( const CUser& otherUser );

		void CopyData( const CUser& otherUser );

		// accessors for private data members
		string GetUsername();
		string GetPassword();
		string GetReaderPassword();
		string GetWriterPassword();
		string GetAdminPassword();
		bool CanWrite();
		bool IsAdmin();
		bool IsValid();

		// modifiers for public data members
		void SetUsername( string newUsername );
		void SetPassword( string newPassword );
		void SetReaderPassword( string newReaderPassword );
		void SetWriterPassword( string newWriterPassword );
		void SetAdminPassword( string newAdminPassword );
		void SetCanWrite( bool newval );
		//void SetIsAdmin ( bool newval );
		void SetIsValid ( bool newval );

	private:
		string username;
		string password;
		string readerPassword;
		string writerPassword;
		string adminPassword;
		bool canWrite; // can the user modify the database's data?
		//bool isAdmin;  // can the user modify the database's metadata? (lists,users)
		bool isValid;
};

#endif