
//#define DECLARE_USER

#include "User.h"

CUser::CUser() {
	username = "";
	password = "";
	readerPassword = "";
	writerPassword = "";
	adminPassword = "";
	canWrite = false;
	//isAdmin = false;
	isValid = false;
}

CUser::CUser( const CUser& otherUser ) {
	CopyData( otherUser );
}

CUser CUser::operator=( const CUser& otherUser ) {
	CopyData( otherUser );
	return *this;
}

void CUser::CopyData( const CUser& otherUser ) {
	username = otherUser.username;
	password = otherUser.password;
	readerPassword = otherUser.readerPassword;
	writerPassword = otherUser.writerPassword;
	adminPassword = otherUser.adminPassword;
	canWrite = otherUser.canWrite;
	//isAdmin = otherUser.isAdmin;
	isValid = otherUser.isValid;
}

string CUser::GetUsername() { return username; }
string CUser::GetPassword() { return password; }
string CUser::GetReaderPassword() { return readerPassword; }
string CUser::GetWriterPassword() { return writerPassword; }
string CUser::GetAdminPassword() { return adminPassword; }
bool CUser::CanWrite() { return canWrite; }
//bool CUser::IsAdmin() { return isAdmin; }
//bool CUser::CanWrite() { return writerPassword!=""; }
bool CUser::IsAdmin() { return username=="admin"; }
bool CUser::IsValid() { return isValid; }

void CUser::SetUsername( string newUsername ) { username = newUsername; }
void CUser::SetPassword( string newPassword ) { password = newPassword; }
void CUser::SetReaderPassword( string newReaderPassword ) { readerPassword = newReaderPassword; }
void CUser::SetWriterPassword( string newWriterPassword ) { writerPassword = newWriterPassword; }
void CUser::SetAdminPassword( string newAdminPassword ) { adminPassword = newAdminPassword; }
void CUser::SetCanWrite( bool newval ) { canWrite = newval; }
//void CUser::SetIsAdmin ( bool newval ) { isAdmin = newval; }
void CUser::SetIsValid ( bool newval ) { isValid = newval; }
