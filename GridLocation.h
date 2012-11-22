
/* 
 * GridLocation.h
 *
 * This class
 *
 */

#ifndef GRIDLOCATION_H
#define GRIDLOCATION_H

class CGridLocation {
	public:
		CGridLocation();
		CGridLocation( int nrow, int ncolumn );

		int GetRow();
		int GetColumn();

		void SetRow( int nRow );
		void SetColumn( int nColumn );
		void SetRowBorder( int nRow );
		void SetColumnBorder( int nColumn );
		void SetOnHeader();
		void SetOnHeaderBorder();

		bool IsValid( int nRows, int nColumns );
		bool IsColumnResizeArea();
		bool IsCell();
	private:
		int  row;
		int  column;
		bool rowValid;
		bool columnValid;
		bool rowBorder;
		bool columnBorder;
		bool onHeader;
		bool onHeaderBorder;

};

#endif
