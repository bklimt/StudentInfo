
//#define DECLARE_STORAGE
#include "StorageController.h"
#include "MainWindow.h"
#include "Preferences.h"
#include "Crypto.h"
#include <objbase.h> // for CoInitialize()

extern CPreferences preferences;
extern CMainWindow* mainWnd;
extern CUser user;

// functions to make the code later easier to type

string MakeTableName( string name ) {
	if ( preferences.GetBool( "UsePrefix" ) ) {
		return "StudentInfoSuperUser." + name;
	} else {
		return name;
	}
}

string GetSQLString( CVariant data ) {
	return data.GetSQLString();
}

CStorageController::CStorageController() {
	CoInitialize( NULL );
}

CStorageController::~CStorageController() {
	CoUninitialize();
}

CIndexedList CStorageController::GetSchoolYearList() {
	return GetList( "SchoolYear" );
	/*
	CTable yearList;
	CIndexedList ans("SchoolYear");

	if ( SafeQuery( yearList, (string)"SELECT * FROM " + MakeTableName("SchoolYear"), "Unable to get year list!" ) ) {
		while ( !yearList.IsAtEOF() ) {
			ans.AddString( yearList[0], yearList[1] );
			yearList.MoveNext();
		}
	}

	return ans;
	*/
}

bool CStorageController::ValidateUser( string username, string password ) {
	CTable rs;
	user.SetIsValid( false );

	if ( !SafeAuthQuery(
		rs,
		(string)"SELECT * FROM " +
			MakeTableName("Auth") +
			" WHERE Username='" + 
			username.c_str() + 
			"'", 
		"Unable to verify username/password for login!" )
	) {
		return false;
	}

	if ( !rs.IsEmpty() ) {
		if ( (string)rs["PasswordHash"] == CCrypto::HashMD5(password) ) {
			user.SetUsername  ( username );
			user.SetPassword  ( password );

			user.SetReaderPassword( CCrypto::DecryptString( rs["encReaderPassword"], password+"r" ) );
			user.SetWriterPassword( CCrypto::DecryptString( rs["encWriterPassword"], password+"w" ) );
			user.SetAdminPassword( CCrypto::DecryptString( rs["encAdminPassword"], password+"a" ) );

			user.SetCanWrite( user.GetWriterPassword() != "" );
			//user.SetCanWrite  ( rs["CanWrite"] );
			//user.SetIsAdmin   ( rs["IsAdmin"] );
			user.SetIsValid   ( true );
		}
	}

	if ( !user.IsValid() ) {
		MessageBox( mainWnd->GetSafeHwnd(), "You entered an incorrect username or password.", "", 0 );
	}

	return user.IsValid();
}

vector<string> CStorageController::GetUserList() {
	CTable rs;
	vector<string> ans;

	if ( SafeQuery( rs, "SELECT * FROM " + MakeTableName("Auth"), "Unable to get user list!" ) ) {
		while ( !rs.IsAtEOF() ) {
			ans.push_back( rs["Username"] );
			rs.MoveNext();
		}
	}

	return ans;
}

CUser CStorageController::GetUser( string username, string adminPassword ) {
	CUser newUser;
	CTable rs;

	if ( !user.IsAdmin() ) {
		return newUser;
	}

	if ( adminPassword == "" ) {
		adminPassword = user.GetPassword();
	}

	if ( SafeQuery( rs, (string)"SELECT * FROM " + MakeTableName("Auth") + " WHERE Username='" + username.c_str() + "'", "Unable to get data for user." ) ) {
		if ( !rs.IsEmpty() ) {
			string password = CCrypto::DecryptString( rs["encPassword"], adminPassword+username );

			newUser.SetUsername( username );
			newUser.SetPassword( password );

			newUser.SetReaderPassword( CCrypto::DecryptString( rs["encReaderPassword"], password+"r" ) );
			newUser.SetWriterPassword( CCrypto::DecryptString( rs["encWriterPassword"], password+"w" ) );
			newUser.SetAdminPassword( CCrypto::DecryptString( rs["encAdminPassword"], password+"a" ) );

			newUser.SetCanWrite( newUser.GetWriterPassword() != "" );
			//user.SetIsAdmin( rs["IsAdmin"] );
			newUser.SetIsValid( true );
		}
	}

	return newUser;
}

bool CStorageController::CreateUser( string username ) {
	if ( !user.IsAdmin() ) {
		return false;
	}

	string s = (string)
		"INSERT INTO " + MakeTableName("Auth") + "(Username) VALUES ('" + username + "')";

	return SafeAdminUpdate( s, "Unable to create the new user." );
}

bool CStorageController::ChangeAdminPassword( string newPassword ) {
	if ( !user.IsAdmin() ) {
		return false;
	}

	string oldPassword = user.GetPassword();
	user.SetPassword( newPassword );

	CTable rs;
	if ( !SafeQuery( rs, "SELECT UserName FROM " + MakeTableName("Auth"), "Unable to update user." ) ) {
		return false;
	}

	while ( !rs.IsAtEOF() ) {
		CUser temp = GetUser( rs["UserName"], oldPassword );
		if ( !temp.IsAdmin() ) {
			UpdateUser( rs["UserName"], temp, newPassword );
		}
		rs.MoveNext();
	}

	return true;
}

bool CStorageController::UpdateUser( string username, CUser newUser, string adminPassword ) {
	if ( !user.IsAdmin() ) {
		return false;
	}

	if ( adminPassword == "" ) {
		adminPassword = user.GetPassword();
	}

	if ( newUser.GetPassword().length() == 0 ) {
		MessageBox( mainWnd->GetSafeHwnd(), "Passwords cannot be blank.", "", 0 );
		return false;
	}

	if ( newUser.IsAdmin() && ( newUser.GetPassword() != adminPassword ) ) {
		if ( !ChangeAdminPassword( newUser.GetPassword() ) ) {
			return false;
		}
		adminPassword = newUser.GetPassword();
	}

	string passwordHash = CCrypto::HashMD5( newUser.GetPassword() );
	string encReaderPassword = CCrypto::EncryptString( user.GetReaderPassword(), newUser.GetPassword()+"r" );
	string encWriterPassword = CCrypto::EncryptString( user.GetWriterPassword(), newUser.GetPassword()+"w" );
	string encAdminPassword = CCrypto::EncryptString( user.GetAdminPassword(), newUser.GetPassword()+"a" );
	string encPassword = CCrypto::EncryptString( newUser.GetPassword(), adminPassword+newUser.GetUsername() );

	if ( !newUser.CanWrite() ) {
		encWriterPassword = "";
	}

	if ( !newUser.IsAdmin() ) {
		encAdminPassword = "";
	}

	string s = (string)
		"UPDATE " + MakeTableName("Auth") + " SET" +
		" Username=" + GetSQLString( newUser.GetUsername() ) +
		", PasswordHash=" + GetSQLString( passwordHash ) +
		", EncReaderPassword=" + GetSQLString( encReaderPassword ) +
		", EncWriterPassword=" + GetSQLString( encWriterPassword ) +
		", EncAdminPassword=" + GetSQLString( encAdminPassword ) +
		", EncPassword=" + GetSQLString( encPassword ) +
		//", CanWrite=" + GetSQLString( newUser.CanWrite() ) +
		//", IsAdmin=" + GetSQLString( newUser.IsAdmin() ) +
		" WHERE Username=" + GetSQLString( username );

	return SafeAdminUpdate( s, "Unable to update this user's data." );
}

bool CStorageController::DeleteUser( string username ) {
	if ( !user.IsAdmin() ) {
		return false;
	}

	if ( username == "admin" || username == "" ) {
		MessageBox( mainWnd->GetSafeHwnd(), "This user cannot be deleted.", "", 0 );
		return false;
	}

	string s = (string)
		"DELETE FROM " + MakeTableName("Auth") + " WHERE Username=" + GetSQLString( username );

	return SafeAdminUpdate( s, "This user could not be deleted." );
}

bool CStorageController::GetStudentList( CTable& table, string first, string last, string ssn, int year ) {
	ssn += "%";
	first += "%";
	last += "%";

	string s = (string)
		"SELECT FirstName, LastName, s.SSN " +
		"FROM " + MakeTableName("Student") + " s, " + MakeTableName("Person") + " p, " + MakeTableName("StudentYear") + " sy " + 
		"WHERE s.PersonID = p.PersonID " +
		"AND sy.SSN = s.SSN " +
		"AND ( firstname LIKE '" + first.c_str() + "' OR FirstName IS NULL ) " +
		"AND ( lastname LIKE '" + last.c_str() + "' OR LastName IS NULL ) " +
		"AND s.SSN LIKE '" + ssn.c_str() + "' " +
		"AND sy.SchoolYearID = " + (string)(CVariant)year;

	if ( !SafeQuery( table, s, "Unable to get list of students!" ) ) {
		return false;
	}

	return true;
}

void CStorageController::GetAssessmentRecord( CConnection* conn, CAssessmentRecord& record, int recordID ) {
	string s = (string)
		"SELECT * " +
		"FROM " + MakeTableName("AssessmentRecord") + " " +
		"WHERE AssessmentRecordID=" + GetSQLString(recordID);

	CTable rs = conn->Execute(s);

	record.achLevel = rs["AchLevelID"];
	record.ardExpectation = rs["ARDExpectationID"];
	record.assessment = rs["AssessmentID"];
	record.assessmentRecordID = rs["AssessmentRecordID"];
	record.baselineAchLevel = rs["BaselineAchLevelID"];
	record.baselineGradeLevel = rs["BaselineGradeLevelID"];
	record.baselineYear = rs["BaselineYearID"];
	record.districtAssessment = rs["DistrictAssessment"];
	record.gradeLevel = rs["GradeLevelID"];

	record.mods.clear();

	s = (string)
		"SELECT * " +
		"FROM " + MakeTableName("AssessmentRecordMod") + " " +
		"WHERE AssessmentRecordID=" + GetSQLString(recordID);

	rs = conn->Execute(s);

	while (!rs.IsAtEOF()) {
		record.mods.push_back( rs["AssessmentModID"] );
		rs.MoveNext();
	}
}

bool CStorageController::GetStudent( CStudent& student, string ssn, int yearID, CConnection* conn ) {
	bool closeConn = false;
	CConnection conn2;
	if ( !conn ) {
		conn = &conn2;
		closeConn = true;
	}
	CTable rs, motherRS, fatherRS;
	CTable evalRS, instructionalServiceRS, relatedServiceRS, esyServiceRS, distRS;
	CTable evaluationRequestRS;
	string s;

	if ( !conn->IsOpen() ) {
		if ( !OpenForReading( conn ) ) {
			MessageBox( mainWnd->GetSafeHwnd(), "Unable to retrieve this student's information!\nUnable to connect to the database", "", 0 );
			return false;
		}
	}

	try {
		s = (string)
			"SELECT * " +
			"FROM " + MakeTableName("Student") + " s, " + MakeTableName("Person") + " p, " + MakeTableName("StudentYear") + " sy " + 
			"WHERE s.PersonID = p.PersonID " +
			"AND s.SSN = sy.SSN " +
			"AND s.SSN='" + ssn + "'";

		rs = conn->Execute( s );

		s = (string)
			"SELECT * " +
			"FROM " + MakeTableName("Person") + " p, " + MakeTableName("StudentContact") + " sc " +
			"WHERE p.PersonID = sc.PersonID " +
			"AND sc.Relationship = 'Mother' " +
			"AND sc.SSN = '" + ssn + "'";

		motherRS = conn->Execute( s );

		s = (string)
			"SELECT * " +
			"FROM " + MakeTableName("Person") + " p, " + MakeTableName("StudentContact") + " sc " +
			"WHERE p.PersonID = sc.PersonID " +
			"AND sc.Relationship = 'Father' " +
			"AND sc.SSN = '" + ssn + "'";

		fatherRS = conn->Execute( s );

		if ( rs.IsAtEOF() ) {
			MessageBox( mainWnd->GetSafeHwnd(), "Unable to retrieve this student's information!", "", 0 );
			conn->Close();
			return false;
		} else {
			student.ssn = rs["SSN"];
			student.self.address = rs["Address"];
			student.self.mailingAddress = rs["MailingAddress"];
			student.self.cellPhone = rs["CellPhone"];
			student.self.city = rs["City"];
			student.self.mailingCity = rs["MailingCity"];
			student.self.emailAddress = rs["Email"];
			student.self.firstName = rs["FirstName"];
			student.self.homePhone = rs["HomePhone"];
			student.self.lastName = rs["LastName"];
			student.self.middleName = rs["MiddleName"];
			student.self.personID = rs["PersonID"];
			student.self.state = rs["State"];
			student.self.mailingState = rs["MailingState"];
			student.self.workPhone = rs["WorkPhone"];
			student.self.zipCode = rs["ZipCode"];
			student.self.mailingZipCode = rs["MailingZipCode"];

			student.father.address = fatherRS["Address"]; 
			student.father.cellPhone = fatherRS["CellPhone"];
			student.father.city = fatherRS["City"];
			student.father.emailAddress = fatherRS["Email"];
			student.father.firstName = fatherRS["FirstName"];
			student.father.homePhone = fatherRS["HomePhone"];
			student.father.lastName = fatherRS["LastName"];
			student.father.middleName = fatherRS["MiddleName"];
			student.father.personID = fatherRS["PersonID"];
			student.father.state = fatherRS["State"];
			student.father.workPhone = fatherRS["WorkPhone"];
			student.father.zipCode = fatherRS["ZipCode"];

			student.mother.address = motherRS["Address"];
			student.mother.cellPhone = motherRS["CellPhone"];
			student.mother.city = motherRS["City"];
			student.mother.emailAddress = motherRS["Email"];
			student.mother.firstName = motherRS["FirstName"];
			student.mother.homePhone = motherRS["HomePhone"];
			student.mother.lastName = motherRS["LastName"];
			student.mother.middleName = motherRS["MiddleName"];
			student.mother.personID = motherRS["PersonID"];
			student.mother.state = motherRS["State"];
			student.mother.workPhone = motherRS["WorkPhone"];
			student.mother.zipCode = motherRS["ZipCode"];

			while ( !rs.IsAtEOF() ) {
				CStudentYear studentYear;

				s = (string)
					"SELECT * " +
					"FROM " + MakeTableName("Evaluation") + " " +
					"WHERE SchoolYearID=" + (string)rs["SchoolYearID"] + " " +
					"AND SSN=" + GetSQLString( ssn );

				evalRS = conn->Execute( s );

				s = (string)
					"SELECT * " +
					"FROM " + MakeTableName("InstructionalService") + " " +
					"WHERE SchoolYearID=" + (string)rs["SchoolYearID"] + " " +
					"AND SSN=" + GetSQLString( ssn );

				instructionalServiceRS = conn->Execute( s );

				s = (string)
					"SELECT * " +
					"FROM " + MakeTableName("RelatedService") + " " +
					"WHERE SchoolYearID=" + (string)rs["SchoolYearID"] + " " +
					"AND SSN=" + GetSQLString( ssn );

				relatedServiceRS = conn->Execute( s );

				s = (string)
					"SELECT * " +
					"FROM " + MakeTableName("ESYService") + " " +
					"WHERE SchoolYearID=" + (string)rs["SchoolYearID"] + " " +
					"AND SSN=" + GetSQLString( ssn );

				esyServiceRS = conn->Execute( s );

				s = (string)
					"SELECT * " +
					"FROM " + MakeTableName("StudentEvaluationRequest") + " " +
					"WHERE SchoolYearID=" + (string)rs["SchoolYearID"] + " " +
					"AND SSN=" + GetSQLString( ssn );

				evaluationRequestRS = conn->Execute( s );

				s = (string)
					"SELECT * " +
					"FROM " + MakeTableName("Distribution") + " " +
					"WHERE SchoolYearID=" + (string)rs["SchoolYearID"] + " " +
					"AND SSN=" + GetSQLString( ssn );

				distRS = conn->Execute( s );

				studentYear.schoolYearID = rs["SchoolYearID"];
				studentYear.ardCompletedDate = rs["ARDCompletedDate"];
				studentYear.assessmentsComments = rs["AssessmentsComments"];
				studentYear.bipDate = rs["BIPDate"];
				studentYear.birthDate = rs["BirthDate"];
				studentYear.campus = rs["CampusID"];
				studentYear.contactsComments  = rs["contactsComments"];
				studentYear.demographicsComments = rs["demographicsComments"];
				studentYear.eligibility = rs["EligibilityID"];
				studentYear.ethnicity = rs["EthnicityID"];
				studentYear.evaluationCompletedDate = rs["EvaluationCompletedDate"];
				studentYear.evaluationsComments = rs["evaluationsComments"];
				studentYear.gender = rs["GenderID"];
				studentYear.grade = rs["GradeID"];
				studentYear.iepEndDate = rs["IEPEndDate"];
				studentYear.iepStartDate = rs["IEPStartDate"];
				studentYear.instructionalSetting = rs["InstructionalSettingID"];
				studentYear.itpDate = rs["ITPDate"];
				studentYear.lastAnnualReviewDate = rs["LastAnnualReviewDate"];
				studentYear.lastARDDate = rs["LastARDDate"];
				studentYear.medicallyFragile = rs["MedicallyFragile"];
				studentYear.medicaid = rs["Medicaid"];
				studentYear.multipleDisabilities = rs["MultipleDisabilities"];
				studentYear.primaryDisability = rs["PrimaryDisabilityID"];
				studentYear.referralDate = rs["ReferralDate"];
				studentYear.referralEligible = rs["ReferralEligibleID"];
				studentYear.referralsComments = rs["referralsComments"];
				studentYear.referringCampus = rs["ReferringCampusID"];
				studentYear.secondaryDisability = rs["SecondaryDisabilityID"];
				studentYear.servicesComments = rs["servicesComments"];
				studentYear.status = rs["StatusID"];
				studentYear.statusDate = rs["StatusDate"];
				studentYear.tertiaryDisability = rs["TertiaryDisabilityID"];

				GetAssessmentRecord( conn, studentYear.k2ReadingAssessmentRecord, rs["k2ReadingAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.readingAssessmentRecord, rs["readingAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.mathAssessmentRecord, rs["mathAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.writingAssessmentRecord, rs["writingAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.scienceAssessmentRecord, rs["scienceAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.socialStudiesAssessmentRecord, rs["socialStudiesAssessmentRecordID"] );
				//GetAssessmentRecord( conn, studentYear.englishIIAssessmentRecord, rs["englishIIAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.englishLAAssessmentRecord, rs["englishLAAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.usHistoryAssessmentRecord, rs["usHistoryAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.algebraIAssessmentRecord, rs["algebraIAssessmentRecordID"] );
				GetAssessmentRecord( conn, studentYear.biologyAssessmentRecord, rs["biologyAssessmentRecordID"] );
				studentYear.assessmentARDDate = rs["AssessmentARDDate"];

				CEvaluation eval;
				while ( !evalRS.IsAtEOF() ) {
					eval.dismissalDate = evalRS["DismissalDate"];
					eval.evaluationEligible = evalRS["EvaluationEligibleID"];
					eval.evaluationDate = evalRS["EvaluationDate"];
					eval.reportDate = evalRS["ReportDate"];
					eval.evaluationType = evalRS["EvaluationTypeID"];
					studentYear.evaluations.push_back( eval );
					evalRS.MoveNext();
				}

				CService service;
				while ( !instructionalServiceRS.IsAtEOF() ) {
					service.specificServiceType = instructionalServiceRS["InstructionalServiceTypeID"];
					service.serviceType = instructionalServiceRS["ServiceTypeID"];
					service.serviceTime = instructionalServiceRS["ServiceTime"];
					studentYear.instructionalServices.push_back( service );
					instructionalServiceRS.MoveNext();
				}

				while ( !relatedServiceRS.IsAtEOF() ) {
					service.specificServiceType = relatedServiceRS["RelatedServiceTypeID"];
					service.serviceType = relatedServiceRS["ServiceTypeID"];
					service.serviceTime = relatedServiceRS["ServiceTime"];
					studentYear.relatedServices.push_back( service );
					relatedServiceRS.MoveNext();
				}

				while ( !esyServiceRS.IsAtEOF() ) {
					service.specificServiceType = esyServiceRS["ESYServiceTypeID"];
					service.serviceType = esyServiceRS["ServiceTypeID"];
					service.serviceTime = esyServiceRS["ServiceTime"];
					studentYear.esyServices.push_back( service );
					esyServiceRS.MoveNext();
				}

				while ( !evaluationRequestRS.IsAtEOF() ) {
					studentYear.evaluationsRequested.push_back( evaluationRequestRS["EvaluationTypeID"] );
					evaluationRequestRS.MoveNext();
				}

				CDistribution dist;
				while ( !distRS.IsAtEOF() ) {
					dist.distributionDate = distRS["DistributionDate"];
					dist.documentID = distRS["DocumentID"];
					dist.distributionMethodID = distRS["DistributionMethodID"];
					dist.initialDistributionID = distRS["InitialDistributionID"];
					studentYear.distributions.push_back( dist );
					distRS.MoveNext();
				}

				studentYear.distributionsComments = rs["distributionsComments"];

				student.years.push_back( studentYear );
				rs.MoveNext();
			}
		}
	} catch( ... ) {
		MessageBox( mainWnd->GetSafeHwnd(), "Unable to retrieve this student's information!", "", 0 );
		return false;
	}

	if ( closeConn ) {
		conn2.Close();
	}
	return true;
}

CIndexedList CStorageController::GetList( string listName, CConnection* conn ) {
	CTable table;
	CIndexedList ans( listName );

	string s = (string)"SELECT * FROM " + MakeTableName(listName) + " ORDER BY " + listName + "ID";
	if ( SafeQuery( table, s, (string)"Unable to get " + listName + " list.", conn ) ) {
		while ( !table.IsAtEOF() ) {
			ans.AddString( table[0], table[1] );
			table.MoveNext();
		}
	}

	return ans;
}

bool CStorageController::StudentExists( string ssn ) {
	CConnection conn;
	if ( !OpenForReading( &conn ) ) {
		MessageBox( 0, "Unable to determine if student exists!", "", 0 );
		return true;
	}

	try {
		string s = (string)"SELECT * FROM " + MakeTableName("Student") + " WHERE SSN=" + GetSQLString(ssn);
		CTable table = conn.Execute( s );
		if ( table.IsAtEOF() ) {
			conn.Close();
			return false;
		} else {
			conn.Close();
			return true;
		}
	}
	catch ( ... ) {
		conn.Close();
		MessageBox( mainWnd->GetSafeHwnd(), "Unable to determine if student exists!", "", 0 );
		return true;
	}
}

bool CStorageController::CreateStudent( string ssn, int schoolYearID ) {
	CConnection conn;
	if ( !conn.Open( preferences.GetString("DSN"), "StudentInfoWriter", user.GetWriterPassword() ) ) {
		MessageBox( mainWnd->GetSafeHwnd(), "Unable to connect to database.", "", 0 );
		return false;
	}

	try {

		conn.BeginTransaction();

		string s = "SELECT MAX(PersonID) AS mp FROM " + MakeTableName("Person");
		CTable table = conn.Execute( s );

		int personid;
		int motherid;
		int fatherid;

		if ( table.IsAtEOF() ) {
			personid = 1;
			motherid = 2;
			fatherid = 3;
		} else {
			personid = ((int)table["mp"]) + 1;
			motherid = ((int)table["mp"]) + 2;
			fatherid = ((int)table["mp"]) + 3;
		}

		s = "SELECT MAX(AssessmentRecordID) AS mar FROM " + MakeTableName("AssessmentRecord");
		table = conn.Execute( s );

		int marid;

		if ( table.IsAtEOF() ) {
			marid = 1;
		} else {
			marid = ((int)table["mar"]) + 1;
		}

		// create the entries in the person table
		char temp[10000];
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("Person") + "(PersonID) VALUES (%d)").c_str(), personid );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("Person") + "(PersonID) VALUES (%d)").c_str(), motherid );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("Person") + "(PersonID) VALUES (%d)").c_str(), fatherid );
		conn.Execute( temp );

		// create the entries in the student table
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("Student") + "(SSN,PersonID) VALUES ('%s',%d)").c_str(), ssn.c_str(), personid );
		conn.Execute( temp );

		// create the entries in the StudentContact table
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("StudentContact") + "(SSN,PersonID,Relationship) VALUES ('%s',%d,'Mother')").c_str(), ssn.c_str(), motherid );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("StudentContact") + "(SSN,PersonID,Relationship) VALUES ('%s',%d,'Father')").c_str(), ssn.c_str(), fatherid );
		conn.Execute( temp );

		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+1 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+2 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+3 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+4 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+5 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+6 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+7 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+8 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+9 );
		conn.Execute( temp );
		sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+10 );
		conn.Execute( temp );

		sprintf( temp, (
			(string)"INSERT INTO " + MakeTableName("StudentYear") + 
			"(SSN,SchoolYearID,"
			"k2ReadingAssessmentRecordID,"
			"readingAssessmentRecordID,"
			"mathAssessmentRecordID,"
			"writingAssessmentRecordID,"
			"scienceAssessmentRecordID,"
			"socialStudiesAssessmentRecordID,"
			"englishIIAssessmentRecordID,"
			"englishLAAssessmentRecordID,"
			"usHistoryAssessmentRecordID,"
			"algebraIAssessmentRecordID,"
			"biologyAssessmentRecordID)"
			" VALUES ('%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)").c_str(),
			ssn.c_str(),
			schoolYearID,
			marid,
			marid+1,
			marid+2,
			marid+3,
			marid+4,
			marid+5,
			marid+6,
			marid+7,
			marid+8,
			marid+9,
			marid+10
			);
		conn.Execute( temp );
		
		/*
		student.years[i].k2ReadingAssessmentRecord.assessmentRecordID = marid;
		student.years[i].readingAssessmentRecord.assessmentRecordID = marid+1;
		student.years[i].mathAssessmentRecord.assessmentRecordID = marid+2;
		student.years[i].writingAssessmentRecord.assessmentRecordID = marid+3;
		student.years[i].scienceAssessmentRecord.assessmentRecordID = marid+4;
		student.years[i].socialStudiesAssessmentRecord.assessmentRecordID = marid+5;
		//student.years[i].englishIIAssessmentRecord.assessmentRecordID = marid+6;
		student.years[i].englishLAAssessmentRecord.assessmentRecordID = marid+7;
		student.years[i].usHistoryAssessmentRecord.assessmentRecordID = marid+8;
		student.years[i].algebraIAssessmentRecord.assessmentRecordID = marid+9;
		student.years[i].biologyAssessmentRecord.assessmentRecordID = marid+10;
		*/

		conn.CommitTransaction();
		return true;
	}
	catch ( ... ) {
		conn.CancelTransaction();
		conn.Close();
		MessageBox( mainWnd->GetSafeHwnd(), "Unable to create student!", "", 0 );
		return false;
	}
}

bool CStorageController::ChangeSSN( string oldSSN, string newSSN ) {
	CConnection conn;
	if ( !conn.Open( preferences.GetString("DSN"), "StudentInfoWriter", user.GetWriterPassword() ) ) {
		MessageBox( 0, "Unable to change student's SSN!", "", 0 );
		return false;
	}

	try {
		conn.BeginTransaction();

		string s = (string)"SELECT * FROM " + MakeTableName("STUDENT") + " WHERE SSN='" + oldSSN + "'";
		CTable studentRecord = conn.Execute( s );
		if ( studentRecord.IsAtEOF() ) {
			conn.CancelTransaction();
			conn.Close();
			MessageBox( 0, "Unable to change student's SSN!", "", 0 );
			return false;
		}

		string personID = (string)( studentRecord[ "PersonID" ] );
		conn.Execute( (string)"INSERT INTO " + MakeTableName("Student") + "(SSN,PersonID) VALUES ('" + newSSN + "'," + personID + ")" );
		conn.Execute( (string)"UPDATE " + MakeTableName("StudentContact") + " SET SSN='" + newSSN + "' where SSN='" + oldSSN + "'" );
		conn.Execute( (string)"UPDATE " + MakeTableName("StudentYear") + " SET SSN='" + newSSN + "' where SSN='" + oldSSN + "'" );
		conn.Execute( (string)"UPDATE " + MakeTableName("Distribution") + " SET SSN='" + newSSN + "' where SSN='" + oldSSN + "'" );
		conn.Execute( (string)"UPDATE " + MakeTableName("Evaluation") + " SET SSN='" + newSSN + "' where SSN='" + oldSSN + "'" );
		conn.Execute( (string)"UPDATE " + MakeTableName("InstructionalService") + " SET SSN='" + newSSN + "' where SSN='" + oldSSN + "'" );
		conn.Execute( (string)"UPDATE " + MakeTableName("RelatedService") + " SET SSN='" + newSSN + "' where SSN='" + oldSSN + "'" );
		conn.Execute( (string)"UPDATE " + MakeTableName("ESYService") + " SET SSN='" + newSSN + "' where SSN='" + oldSSN + "'" );
		conn.Execute( (string)"UPDATE " + MakeTableName("StudentEvaluationRequest") + " SET SSN='" + newSSN + "' where SSN='" + oldSSN + "'" );
		conn.Execute( (string)"DELETE FROM " + MakeTableName("Student") + " WHERE SSN='" + oldSSN + "'" );

		conn.CommitTransaction();
		return true;
	}
	catch ( ... ) {
		conn.CancelTransaction();
		conn.Close();
		MessageBox( 0, "Unable to change student's SSN!", "", 0 );
		return false;
	}
	return false;
}

void CStorageController::UpdatePerson( string personID, CPerson& person, CConnection& conn ) {
	conn.Execute( (string)
		"UPDATE " + MakeTableName("Person") + " SET " +
		"FirstName=" + GetSQLString(person.firstName) + ", " +
		"LastName=" + GetSQLString(person.lastName) + ", " +
		"MiddleName=" + GetSQLString(person.middleName) + ", " +
		"Address=" + GetSQLString(person.address) + ", " +
		"City=" + GetSQLString(person.city) + ", " +
		"State=" + GetSQLString(person.state) + ", " +
		"ZipCode=" + GetSQLString(person.zipCode) + ", " +
		"MailingAddress=" + GetSQLString(person.mailingAddress) + ", " +
		"MailingCity=" + GetSQLString(person.mailingCity) + ", " +
		"MailingState=" + GetSQLString(person.mailingState) + ", " +
		"MailingZipCode=" + GetSQLString(person.mailingZipCode) + ", " +
		"HomePhone=" + GetSQLString(person.homePhone) + ", " +
		"WorkPhone=" + GetSQLString(person.workPhone) + ", " +
		"CellPhone=" + GetSQLString(person.cellPhone) + ", " +
		"Email=" + GetSQLString(person.emailAddress) +
		" WHERE PersonID=" + personID
	);
}

void CStorageController::UpdateAssessmentRecord( CConnection* conn, CAssessmentRecord& record ) {
	conn->Execute( (string)
		"UPDATE " + MakeTableName("AssessmentRecord") + " SET " +

		"AssessmentID=" + GetSQLString(record.assessment) + ", " + 
		"BaselineYearID=" + GetSQLString(record.baselineYear) + ", " + 
		"BaselineGradeLevelID=" + GetSQLString(record.baselineGradeLevel) + ", " + 
		"BaselineAchLevelID=" + GetSQLString(record.baselineAchLevel) + ", " + 
		"ARDExpectationID=" + GetSQLString(record.ardExpectation) + ", " + 
		"GradeLevelID=" + GetSQLString(record.gradeLevel) + ", " + 
		"AchLevelID=" + GetSQLString(record.achLevel) + ", " + 
		"DistrictAssessment=" + GetSQLString(record.districtAssessment) + " " +

		"WHERE AssessmentRecordID=" + GetSQLString(record.assessmentRecordID)
	);

	conn->Execute( (string)
		"DELETE FROM " + MakeTableName("AssessmentRecordMod") +
		" WHERE AssessmentRecordID=" + GetSQLString(record.assessmentRecordID)

	);

	for ( int i=0; i<record.mods.size(); i++ ) {
		conn->Execute( (string)
			"INSERT INTO " + MakeTableName("AssessmentRecordMod") +
			"(AssessmentRecordID,AssessmentModID) VALUES (" +
			GetSQLString(record.assessmentRecordID) + "," +
			GetSQLString(record.mods[i]) + ")"
		);
	}
}

bool CStorageController::UpdateStudent( CStudent& student ) {
	CConnection conn;
	if ( !conn.Open( preferences.GetString("DSN"), "StudentInfoWriter", user.GetWriterPassword() ) ) {
		MessageBox( 0, "Unable to update student's data!", "", 0 );
		return false;
	}

	try {
		conn.BeginTransaction();

		CTable studentRS = conn.Execute((string)"SELECT * FROM " + MakeTableName("Student") + " WHERE SSN='" + student.ssn + "'" );

		CTable motherRS = conn.Execute((string)
			"SELECT * FROM " + MakeTableName("StudentContact") + " WHERE SSN='" + student.ssn + "' AND Relationship='Mother'"
		);

		CTable fatherRS = conn.Execute((string)
			"SELECT * FROM " + MakeTableName("StudentContact") + " WHERE SSN='" + student.ssn + "' AND Relationship='Father'"
		);

		if ( studentRS.IsAtEOF() || motherRS.IsAtEOF() || fatherRS.IsAtEOF() ) {
			conn.CancelTransaction();
			conn.Close();
			MessageBox( 0, "Unable to update student's data!", "", 0 );
			return false;
		}

		string personID = (string)( studentRS[ "PersonID" ] );
		string motherID = (string)( motherRS[ "PersonID" ] );
		string fatherID = (string)( fatherRS[ "PersonID" ] );

		UpdatePerson( personID, student.self, conn );
		UpdatePerson( motherID, student.mother, conn );
		UpdatePerson( fatherID, student.father, conn );

		for ( int i=0; i<student.years.size(); i++ ) {
			// see if a record already exits for this student and year
			CTable table = conn.Execute( (string)
				"SELECT * FROM " + MakeTableName("StudentYear") + " " + 
				"WHERE SSN='" + student.ssn + "' " +
				"AND SchoolYearID=" + (string)((CVariant)student.years[i].schoolYearID)
			);

			string s;
			if ( table.IsAtEOF() ) {
				s = "SELECT MAX(AssessmentRecordID) AS mar FROM " + MakeTableName("AssessmentRecord");
				CTable table2 = conn.Execute( s );

				int marid;

				if ( table2.IsAtEOF() ) {
					marid = 1;
				} else {
					marid = ((int)table2["mar"]) + 1;
				}

				if ( marid < 1 ) marid=1;

				char temp[1000];
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+1 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+2 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+3 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+4 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+5 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+6 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+7 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+8 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+9 );
				conn.Execute( temp );
				sprintf( temp, ((string)"INSERT INTO " + MakeTableName("AssessmentRecord") + "(AssessmentRecordID) VALUES (%d)").c_str(), marid+10 );
				conn.Execute( temp );

				sprintf( temp, (
					(string)"INSERT INTO " + MakeTableName("StudentYear") + 
					"(SSN,SchoolYearID,"
					"k2ReadingAssessmentRecordID,"
					"readingAssessmentRecordID,"
					"mathAssessmentRecordID,"
					"writingAssessmentRecordID,"
					"scienceAssessmentRecordID,"
					"socialStudiesAssessmentRecordID,"
					"englishIIAssessmentRecordID,"
					"englishLAAssessmentRecordID,"
					"usHistoryAssessmentRecordID,"
					"algebraIAssessmentRecordID,"
					"biologyAssessmentRecordID)"
					" VALUES (%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)").c_str(),
					GetSQLString(student.ssn).c_str(),
					GetSQLString(student.years[i].schoolYearID).c_str(),
					marid,
					marid+1,
					marid+2,
					marid+3,
					marid+4,
					marid+5,
					marid+6,
					marid+7,
					marid+8,
					marid+9,
					marid+10
				);

				student.years[i].k2ReadingAssessmentRecord.assessmentRecordID = marid;
				student.years[i].readingAssessmentRecord.assessmentRecordID = marid+1;
				student.years[i].mathAssessmentRecord.assessmentRecordID = marid+2;
				student.years[i].writingAssessmentRecord.assessmentRecordID = marid+3;
				student.years[i].scienceAssessmentRecord.assessmentRecordID = marid+4;
				student.years[i].socialStudiesAssessmentRecord.assessmentRecordID = marid+5;
				//student.years[i].englishIIAssessmentRecord.assessmentRecordID = marid+6;
				student.years[i].englishLAAssessmentRecord.assessmentRecordID = marid+7;
				student.years[i].usHistoryAssessmentRecord.assessmentRecordID = marid+8;
				student.years[i].algebraIAssessmentRecord.assessmentRecordID = marid+9;
				student.years[i].biologyAssessmentRecord.assessmentRecordID = marid+10;

				conn.Execute( temp );
			} else {
				student.years[i].k2ReadingAssessmentRecord.assessmentRecordID = table["k2ReadingAssessmentRecordID"];
				student.years[i].readingAssessmentRecord.assessmentRecordID = table["readingAssessmentRecordID"];
				student.years[i].mathAssessmentRecord.assessmentRecordID = table["mathAssessmentRecordID"];
				student.years[i].writingAssessmentRecord.assessmentRecordID = table["writingAssessmentRecordID"];
				student.years[i].scienceAssessmentRecord.assessmentRecordID = table["scienceAssessmentRecordID"];
				student.years[i].socialStudiesAssessmentRecord.assessmentRecordID = table["socialStudiesAssessmentRecordID"];
				//student.years[i].englishIIAssessmentRecord.assessmentRecordID = table["englishIIAssessmentRecordID"];
				student.years[i].englishLAAssessmentRecord.assessmentRecordID = table["englishLAAssessmentRecordID"];
				student.years[i].usHistoryAssessmentRecord.assessmentRecordID = table["usHistoryAssessmentRecordID"];
				student.years[i].algebraIAssessmentRecord.assessmentRecordID = table["algebraIAssessmentRecordID"];
				student.years[i].biologyAssessmentRecord.assessmentRecordID = table["biologyAssessmentRecordID"];
			}

			s = (string)
				"UPDATE " + MakeTableName("StudentYear") + " SET " +
				"GradeID=" + GetSQLString(student.years[i].grade) + ", " +
				"StatusID=" + GetSQLString(student.years[i].status) + ", " +
				"EligibilityID=" + GetSQLString(student.years[i].eligibility) + ", " +
				"CampusID=" + GetSQLString(student.years[i].campus) + ", " +
				"GenderID=" + GetSQLString(student.years[i].gender) + ", " +
				"EthnicityID=" + GetSQLString(student.years[i].ethnicity) + ", " +
				"InstructionalSettingID=" + GetSQLString(student.years[i].instructionalSetting) + ", " +
				"ReferringCampusID=" + GetSQLString(student.years[i].referringCampus) + ", " +
				"PrimaryDisabilityID=" + GetSQLString(student.years[i].primaryDisability) + ", " +
				"SecondaryDisabilityID=" + GetSQLString(student.years[i].secondaryDisability) + ", " +
				"TertiaryDisabilityID=" + GetSQLString(student.years[i].tertiaryDisability) + ", " +

				"AssessmentARDDate=" + GetSQLString(student.years[i].assessmentARDDate) + ", " +
				"StatusDate=" + GetSQLString(student.years[i].statusDate) + ", " +
				"BirthDate=" + GetSQLString(student.years[i].birthDate) + ", " +
				"LastARDDate=" + GetSQLString(student.years[i].lastARDDate) + ", " +
				"LastAnnualReviewDate=" + GetSQLString(student.years[i].lastAnnualReviewDate) + ", " +
				"IEPStartDate=" + GetSQLString(student.years[i].iepStartDate) + ", " +
				"IEPEndDate=" + GetSQLString(student.years[i].iepEndDate) + ", " +
				"BIPDate=" + GetSQLString(student.years[i].bipDate) + ", " +
				"ITPDate=" + GetSQLString(student.years[i].itpDate) + ", " +
				"ReferralDate=" + GetSQLString(student.years[i].referralDate) + ", " +
				"EvaluationCompletedDate=" + GetSQLString(student.years[i].evaluationCompletedDate) + ", " +
				"ARDCompletedDate=" + GetSQLString(student.years[i].ardCompletedDate) + ", " +
				"ReferralEligibleID=" + GetSQLString(student.years[i].referralEligible) + ", " +
				"MedicallyFragile=" + GetSQLString(student.years[i].medicallyFragile) + ", " +
				"MultipleDisabilities=" + GetSQLString(student.years[i].multipleDisabilities) + ", " +
				"Medicaid=" + GetSQLString(student.years[i].medicaid) + " " +

				"WHERE SSN=" + GetSQLString(student.ssn) + " " +
				"AND SchoolYearID=" + GetSQLString(student.years[i].schoolYearID);

			conn.Execute( s );

			UpdateAssessmentRecord( &conn, student.years[i].k2ReadingAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].readingAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].mathAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].writingAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].scienceAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].socialStudiesAssessmentRecord );
			//UpdateAssessmentRecord( &conn, student.years[i].englishIIAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].englishLAAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].usHistoryAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].algebraIAssessmentRecord );
			UpdateAssessmentRecord( &conn, student.years[i].biologyAssessmentRecord );

			s = (string)
				"UPDATE " + MakeTableName("StudentYear") + " SET " +
				"DemographicsComments=" + GetSQLString(student.years[i].demographicsComments) + ", " +
				"ContactsComments=" + GetSQLString(student.years[i].contactsComments) + ", " +
				"EvaluationsComments=" + GetSQLString(student.years[i].evaluationsComments) + ", " +
				"DistributionsComments=" + GetSQLString(student.years[i].distributionsComments) + ", " +
				"ServicesComments=" + GetSQLString(student.years[i].servicesComments) + ", " +
				"AssessmentsComments=" + GetSQLString(student.years[i].assessmentsComments) + ", " +
				"ReferralsComments=" + GetSQLString(student.years[i].referralsComments) + " " +

				"WHERE SSN=" + GetSQLString(student.ssn) + " " +
				"AND SchoolYearID=" + GetSQLString(student.years[i].schoolYearID);
			conn.Execute( s );

			int j;

			s = (string)
				"DELETE FROM " + MakeTableName("Evaluation") + " " +
				"WHERE SSN=" + GetSQLString( student.ssn ) + " " +
				"AND SchoolYearID=" + GetSQLString( student.years[i].schoolYearID )
			;
			conn.Execute( s );

			int maxid = 1;
			table = conn.Execute( "SELECT MAX(EvaluationID) as maxid FROM " + MakeTableName("EVALUATION") );
			if ( !table.IsAtEOF() ) {
				maxid = (int)table["maxid"];
			}

			for ( j=0; j<student.years[i].evaluations.size(); j++ ) {
				s = (string)
					"INSERT INTO " + MakeTableName("EVALUATION") + "( "
					"EvaluationID,"
					"EvaluationTypeID,"
					"SSN,"
					"SchoolYearID,"
					"EvaluationDate,"
					"ReportDate,"
					"EvaluationEligibleID,"
					"DismissalDate ) VALUES ( " +
					GetSQLString( maxid+j+1 ) + ", " +
					GetSQLString( student.years[i].evaluations[j].evaluationType ) + ", " +
					GetSQLString( student.ssn ) + ", " +
					GetSQLString( student.years[i].schoolYearID ) + ", " +
					GetSQLString( student.years[i].evaluations[j].evaluationDate ) + ", " +
					GetSQLString( student.years[i].evaluations[j].reportDate ) + ", " +
					GetSQLString( student.years[i].evaluations[j].evaluationEligible ) + ", " +
					GetSQLString( student.years[i].evaluations[j].dismissalDate ) + " )"
				;
				conn.Execute( s );
			}

			s = (string)
				"DELETE FROM " + MakeTableName("ESYService") + " " +
				"WHERE SSN=" + GetSQLString( student.ssn ) + " " +
				"AND SchoolYearID=" + GetSQLString( student.years[i].schoolYearID )
			;
			conn.Execute( s );

			maxid = 1;
			table = conn.Execute( "SELECT MAX(ESYServiceID) as maxid FROM " + MakeTableName("ESYService") );
			if ( !table.IsAtEOF() ) {
				maxid = (int)table["maxid"];
			}

			for ( j=0; j<student.years[i].esyServices.size(); j++ ) {
				s = (string)
					"INSERT INTO " + MakeTableName("ESYService") + " ( "
					"ESYServiceID,"
					"ESYServiceTypeID,"
					"SSN,"
					"SchoolYearID,"
					"ServiceTypeID,"
					"ServiceTime ) VALUES ( " +
					GetSQLString( maxid+j+1 ) + ", " +
					GetSQLString( student.years[i].esyServices[j].specificServiceType ) + ", " +
					GetSQLString( student.ssn ) + ", " +
					GetSQLString( student.years[i].schoolYearID ) + ", " +
					GetSQLString( student.years[i].esyServices[j].serviceType ) + ", " +
					GetSQLString( student.years[i].esyServices[j].serviceTime ) + " )"
				;
				conn.Execute( s );
			}

			s = (string)
				"DELETE FROM " + MakeTableName("RelatedService") + " " +
				"WHERE SSN=" + GetSQLString( student.ssn ) + " " +
				"AND SchoolYearID=" + GetSQLString( student.years[i].schoolYearID )
			;
			conn.Execute( s );

			maxid = 1;
			table = conn.Execute( "SELECT MAX(RelatedServiceID) as maxid FROM " + MakeTableName("RelatedService") );
			if ( !table.IsAtEOF() ) {
				maxid = (int)table["maxid"];
			}

			for ( j=0; j<student.years[i].relatedServices.size(); j++ ) {
				s = (string)
					"INSERT INTO " + MakeTableName("RelatedService") + " ( "
					"RelatedServiceID,"
					"RelatedServiceTypeID,"
					"SSN,"
					"SchoolYearID,"
					"ServiceTypeID,"
					"ServiceTime ) VALUES ( " +
					GetSQLString( maxid+j+1 ) + ", " +
					GetSQLString( student.years[i].relatedServices[j].specificServiceType ) + ", " +
					GetSQLString( student.ssn ) + ", " +
					GetSQLString( student.years[i].schoolYearID ) + ", " +
					GetSQLString( student.years[i].relatedServices[j].serviceType ) + ", " +
					GetSQLString( student.years[i].relatedServices[j].serviceTime ) + " )"
				;
				conn.Execute( s );
			}

			s = (string)
				"DELETE FROM " + MakeTableName("InstructionalService") + " " +
				"WHERE SSN=" + GetSQLString( student.ssn ) + " " +
				"AND SchoolYearID=" + GetSQLString( student.years[i].schoolYearID )
			;
			conn.Execute( s );

			maxid = 1;
			table = conn.Execute( "SELECT MAX(InstructionalServiceID) as maxid FROM " + MakeTableName("InstructionalService") );
			if ( !table.IsAtEOF() ) {
				maxid = (int)table["maxid"];
			}

			for ( j=0; j<student.years[i].instructionalServices.size(); j++ ) {
				s = ( (string)
					"INSERT INTO " + MakeTableName("InstructionalService") + " ( " +
					"InstructionalServiceID," +
					"InstructionalServiceTypeID," +
					"SSN," +
					"SchoolYearID," +
					"ServiceTypeID," +
					"ServiceTime ) VALUES ( " +
					GetSQLString( maxid+j+1 ) + ", " +
					GetSQLString( student.years[i].instructionalServices[j].specificServiceType ) + ", " +
					GetSQLString( student.ssn ) + ", " +
					GetSQLString( student.years[i].schoolYearID ) + ", " +
					GetSQLString( student.years[i].instructionalServices[j].serviceType ) + ", " +
					GetSQLString( student.years[i].instructionalServices[j].serviceTime ) + " )"
				);
				//MessageBox( mainWnd->GetSafeHwnd(), s.c_str() );
				conn.Execute(s);
			}

			s = (string)
				"DELETE FROM " + MakeTableName("StudentEvaluationRequest") + " " +
				"WHERE SSN=" + GetSQLString( student.ssn ) + " " +
				"AND SchoolYearID=" + GetSQLString( student.years[i].schoolYearID )
			;
			conn.Execute( s );
			for ( j=0; j<student.years[i].evaluationsRequested.size(); j++ ) {
				string s = ( (string)
					"INSERT INTO " + MakeTableName("StudentEvaluationRequest") + " ( " +
					"EvaluationTypeID," +
					"SSN," +
					"SchoolYearID ) VALUES ( " +
					GetSQLString( student.years[i].evaluationsRequested[j] ) + ", " +
					GetSQLString( student.ssn ) + ", " +
					GetSQLString( student.years[i].schoolYearID ) + " )"
				);
				conn.Execute( s );
			}

			s = (string)
				"DELETE FROM " + MakeTableName("Distribution") + " " +
				"WHERE SSN=" + GetSQLString( student.ssn ) + " " +
				"AND SchoolYearID=" + GetSQLString( student.years[i].schoolYearID )
			;
			conn.Execute( s );

			maxid = 1;
			table = conn.Execute( "SELECT MAX(DistributionID) as maxid FROM " + MakeTableName("Distribution") );
			if ( !table.IsAtEOF() ) {
				maxid = (int)table["maxid"];
			}
			maxid = ( maxid<1 ? 1 : maxid );

			for ( j=0; j<student.years[i].distributions.size(); j++ ) {
				s = (string)
					"INSERT INTO " + MakeTableName("Distribution") + " ( "
					"DistributionID,"
					"SSN,"
					"SchoolYearID,"
					"DistributionDate,"
					"DocumentID,"
					"DistributionMethodID,"
					"InitialDistributionID ) VALUES ( " +
					GetSQLString( maxid+j+1 ) + ", " +
					GetSQLString( student.ssn ) + ", " +
					GetSQLString( student.years[i].schoolYearID ) + ", " +
					GetSQLString( student.years[i].distributions[j].distributionDate ) + ", " +
					GetSQLString( student.years[i].distributions[j].documentID ) + ", " +
					GetSQLString( student.years[i].distributions[j].distributionMethodID ) + ", " +
					GetSQLString( student.years[i].distributions[j].initialDistributionID ) + " )"
				;
				conn.Execute( s );
			}
		}

		conn.CommitTransaction();
		return true;
	}
	catch ( ... ) {
		conn.CancelTransaction();
		conn.Close();
		MessageBox( 0, "Unable to update student's data!", "", 0 );
		return false;
	}
	return false;
}

bool CStorageController::DeleteStudentYear( string ssn, int schoolYearID ) {
	CConnection conn;
	if ( !OpenForWriting( &conn ) ) {
		MessageBox( 0, "Unable to delete student!", "", 0 );
		return false;
	}

	try {
		conn.BeginTransaction();

		int arid1 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["k2ReadingAssessmentRecordID"];
		int arid2 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["readingAssessmentRecordID"];
		int arid3 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["mathAssessmentRecordID"];
		int arid4 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["writingAssessmentRecordID"];
		int arid5 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["englishIIAssessmentRecordID"];
		int arid6 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["englishLAAssessmentRecordID"];
		int arid7 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["socialStudiesAssessmentRecordID"];
		int arid8 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["algebraIAssessmentRecordID"];
		int arid9 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["usHistoryAssessmentRecordID"];
		int arid10 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["biologyAssessmentRecordID"];
		int arid11 = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID))["scienceAssessmentRecordID"];

		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid1));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid2));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid3));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid4));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid5));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid6));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid7));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid8));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid9));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid10));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecordMod") + " WHERE AssessmentRecordID=" + GetSQLString(arid11));

		conn.Execute((string)"DELETE FROM " + MakeTableName("Distribution") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID) );
		conn.Execute((string)"DELETE FROM " + MakeTableName("Evaluation") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID) );
		conn.Execute((string)"DELETE FROM " + MakeTableName("ESYService") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID) );
		conn.Execute((string)"DELETE FROM " + MakeTableName("RelatedService") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID) );
		conn.Execute((string)"DELETE FROM " + MakeTableName("InstructionalService") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID) );
		conn.Execute((string)"DELETE FROM " + MakeTableName("StudentEvaluationRequest") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID) );
		conn.Execute((string)"DELETE FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) + " AND SchoolYearID=" + GetSQLString(schoolYearID) );

		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid1));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid2));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid3));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid4));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid5));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid6));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid7));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid8));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid9));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid10));
		conn.Execute((string)"DELETE FROM " + MakeTableName("AssessmentRecord") + " WHERE AssessmentRecordID=" + GetSQLString(arid11));

		CTable table = conn.Execute( (string)"SELECT * FROM " + MakeTableName("StudentYear") + " WHERE SSN=" + GetSQLString(ssn) );
		if ( table.IsAtEOF() ) {
			CTable personRS = conn.Execute((string)"SELECT * FROM " + MakeTableName("StudentContact") + " WHERE SSN=" + GetSQLString(ssn) );
			conn.Execute( (string)"DELETE FROM " + MakeTableName("StudentContact") + " WHERE SSN=" + GetSQLString(ssn) );
			while ( !personRS.IsAtEOF() ) {
				conn.Execute( (string)"DELETE FROM " + MakeTableName("Person") + " WHERE PersonID=" + (string)personRS["PersonID"] );
				personRS.MoveNext();
			}

			CTable studentRS = conn.Execute((string)"SELECT * FROM " + MakeTableName("Student") + " WHERE SSN=" + GetSQLString(ssn) );
			conn.Execute( (string)"DELETE FROM " + MakeTableName("Student") + " WHERE SSN=" + GetSQLString(ssn) );
			string s = "DELETE FROM " + MakeTableName("Person") + " WHERE PersonID=" + (string)studentRS["PersonID"];
			conn.Execute( s );
		}

		conn.CommitTransaction();
		return true;
	}
	catch ( ... ) {
		conn.CancelTransaction();
		conn.Close();
		MessageBox( mainWnd->CMainWindow::GetSafeHwnd(), "Unable to delete student!", "", 0 );
		return false;
	}
	return false;
}

bool CStorageController::RemoveListItem( string listName, int index ) {
	CConnection conn;
	if ( !OpenForWriting( &conn ) ) {
		MessageBox( mainWnd->GetSafeHwnd(), "Unable to connect to the database.", "StudentInfo", 0 );
		return false;
	}
	try {
		string s = ( (string)"DELETE FROM " + MakeTableName(listName) + " WHERE " + listName + "ID=" + GetSQLString( index ) );
		conn.Execute( s );
	}
	catch ( ... ) {
		MessageBox( mainWnd->GetSafeHwnd(), "This list entry could not be deleted.  It is probably being used for a student.", "StudentInfo", 0 );
		conn.Close();
		return false;
	}
	conn.Close();
	return true;
}

bool CStorageController::AddListItem( string listName, int index, string value ) {
	CConnection conn;
	if ( !OpenForWriting( &conn ) ) {
		MessageBox( mainWnd->GetSafeHwnd(), "Unable to connect to the database.", "StudentInfo", 0 );
		return false;
	}
	try {
		string s = (
			(string)"INSERT INTO " + MakeTableName(listName) + "(" +
			listName + "ID, " + listName + "Desc ) VALUES (" +
			GetSQLString( index ) + ", " +
			GetSQLString( value ) + " )"
		);
		conn.Execute( s );
	}
	catch ( ... ) {
		MessageBox( mainWnd->GetSafeHwnd(), "Unable to add the item to the list.", "StudentInfo", 0 );
		conn.Close();
		return false;
	}
	conn.Close();
	return true;
}

bool CStorageController::ModifyListItem( string listName, int index, string value ) {
	CConnection conn;
	if ( !OpenForWriting( &conn ) ) {
		return false;
	}
	try {
		string s = (
			(string)"UPDATE " + MakeTableName(listName) + " " +
			"SET " + listName + "Desc=" + GetSQLString( value ) + " " +
			"WHERE " + listName + "ID=" + GetSQLString( index )
		);
		conn.Execute( s );
	}
	catch ( ... ) {
		conn.Close();
		return false;
	}
	conn.Close();
	return true;
}

bool CStorageController::OpenForAuth( CConnection* conn ) {
	return conn->Open( preferences.GetString( "DSN" ), "StudentInfoAuthReader", "StudentInfoAuthReader" );
}

bool CStorageController::OpenForReading( CConnection* conn ) {
	return conn->Open( preferences.GetString( "DSN" ), "StudentInfoReader", user.GetReaderPassword() );
}

bool CStorageController::OpenForWriting( CConnection* conn ) {
	return conn->Open( preferences.GetString( "DSN" ), "StudentInfoWriter", user.GetWriterPassword() );
}

bool CStorageController::OpenForAdmin( CConnection* conn ) {
	return conn->Open( preferences.GetString( "DSN" ), "StudentInfoAdmin", user.GetAdminPassword() );
}

bool CStorageController::SafeAuthQuery( CTable& table, string sql, string error, CConnection* conn ) {
	CConnection* connPtr = conn;
	// if conn is null, create a new connection for this call only
	CConnection conn2;
	if ( !connPtr ) {
		connPtr = &conn2;
	}
	if ( !connPtr->IsOpen() ) {
		if ( !OpenForAuth( connPtr ) ) {
			MessageBox( 0, "Unable to connect to the database.", "Error", 0 );
			return false;
		}
	}
	try {
		table = connPtr->Execute( sql );
	}
	catch (...) {
		MessageBox( 0, error.c_str(), "Error", 0 );
		connPtr->Close();
		return false;
	}
	// delete the temporary connection
	if ( !conn ) {
		conn2.Close();
	}
	return true;
}

bool CStorageController::SafeQuery( CTable& table, string sql, string error, CConnection* conn ) {
	CConnection* connPtr = conn;
	// if conn is null, create a new connection for this call only
	CConnection conn2;
	if ( !connPtr ) {
		connPtr = &conn2;
	}
	if ( !connPtr->IsOpen() ) {
		if ( !OpenForReading( connPtr ) ) {
			MessageBox( 0, "Unable to connect to the database.", "Error", 0 );
			return false;
		}
	}
	try {
		table = connPtr->Execute( sql );
	}
	catch (...) {
		MessageBox( 0, error.c_str(), "Error", 0 );
		connPtr->Close();
		return false;
	}
	// delete the temporary connection
	if ( !conn ) {
		conn2.Close();
	}
	return true;
}

bool CStorageController::SafeUpdate( string sql, string error ) {
	CConnection conn;
	if ( !OpenForWriting( &conn ) ) {
		MessageBox( 0, "Unable to connect to the database.", "Error", 0 );
		return false;
	}
	try {
		conn.Execute( sql );
	}
	catch (...) {
		MessageBox( 0, error.c_str(), "Error", 0 );
		conn.Close();
		return false;
	}
	conn.Close();
	return true;
}

bool CStorageController::SafeAdminUpdate( string sql, string error ) {
	CConnection conn;
	if ( !OpenForAdmin( &conn ) ) {
		MessageBox( 0, "Unable to connect to the database.", "Error", 0 );
		return false;
	}
	try {
		conn.Execute( sql );
	}
	catch (...) {
		MessageBox( 0, error.c_str(), "Error", 0 );
		conn.Close();
		return false;
	}
	conn.Close();
	return true;
}

