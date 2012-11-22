
#include "GridLocation.h"

CGridLocation::CGridLocation() {
	row = column = -1;
	rowValid = columnValid = rowBorder = columnBorder = onHeader = onHeaderBorder = false;
}

CGridLocation::CGridLocation( int nRow, int nColumn ) {
	row = nRow;
	column = nColumn;
	rowValid = columnValid = true;
	rowBorder = columnBorder = false;
	onHeaderBorder = onHeader = false;
}

int CGridLocation::GetRow() {
	return row;
}

int CGridLocation::GetColumn() {
	return column;
}

void CGridLocation::SetRow( int nRow ) {
	if ( nRow >= 0 ) {
		row = nRow;
		rowValid = true;
	}
}

void CGridLocation::SetColumn( int nColumn ) {
	if ( nColumn >= 0 ) {
		column = nColumn;
		columnValid = true;
	}
}

void CGridLocation::SetRowBorder( int nRow ) {
	SetRow( nRow );
	if ( row == nRow ) {
		rowBorder = true;
	}
}

void CGridLocation::SetColumnBorder( int nColumn ) {
	SetColumn( nColumn );
	if ( column == nColumn ) {
		columnBorder = true;
	}
}

void CGridLocation::SetOnHeader() {
	onHeader = true;
}

void CGridLocation::SetOnHeaderBorder() {
	onHeader = true;
	onHeaderBorder = true;
}

bool CGridLocation::IsValid( int nRows, int nColumns ) {
	return ( row < nRows && column < nColumns && IsCell() );
}

bool CGridLocation::IsColumnResizeArea() {
	return  onHeader && !onHeaderBorder && !rowValid && columnValid && !rowBorder &&  columnBorder;
}

bool CGridLocation::IsCell() {
	return !onHeader && !onHeaderBorder &&  rowValid && columnValid && !rowBorder && !columnBorder;
}
