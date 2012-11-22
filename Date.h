
#ifndef DATE_H
#define DATE_H

#include <afx.h>
#include <string>
using namespace std;

class CDate {
	public:
		CDate();
		CDate( int newMonth, int newDay, int newYear );
		CDate( string date );
		CDate( int num );
		CDate( const CDate& otherDate );
		CDate operator=( const CDate& otherDate );
		CDate operator+( const int days );
		bool operator==( const CDate& otherDate ) const;
		operator string() const;
		operator int() const;

		bool IsNull();

		int GetYear();
		int GetYearDiff( const CDate& otherDate ) const;

		static bool IsLeapYear( int year );
		static int GetDaysInYear( int year );
		static int GetDaysInMonth( int month, int year );
		static CDate Today();
	private:
		bool isNull;
		int month, day, year;
};

#endif