
/* 
 * GridControl.h
 *
 * This class is a grid that resembles a spreadsheet.
 *   It is similar to Java's JTable or MacOS X's NSTableView?.
 *   I don't know why it isn't part of Windows.
 *
 */

// make sure this file is not included twice
#ifndef GRIDCONTROL_H
#define GRIDCONTROL_H

// disable the bogus warning about STL having long identifiers
#pragma warning(disable:4786)
#include <afxwin.h>               // for MFC
#include <string>
#include <vector>
#include "MemoryDC.h"             // for painting
#include "GridLocation.h"         // this class represents a location in/on the Grid
#include "StringEditHelper.h"     // for handling strings
#include "DateEditHelper.h"       // for handling dates
#include "IndexedListBoxHelper.h" // for handling lists
#include "Variant.h"              // the data in each cell is stored as a CVariant

class CGridControl : public CWnd {
	public:
		// constructors
		CGridControl( bool isReadOnly=false, int rowHeight=21, int columnWidth=80 );
		~CGridControl();

		// overloaded operators
		//   returns the data at a particular location
		CVariant& operator[]( CGridLocation index );

		// needed to handle tab and enter keys correctly
		BOOL PreTranslateMessage( MSG* pMsg );

		// event handlers
		//   these define how the control behaves
		void OnPaint();
		void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
		void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
		void OnLButtonDown( UINT nFlags, CPoint point );
		void OnLButtonUp( UINT nFlags, CPoint point );
		void OnLButtonDblClk( UINT nFlags, CPoint point );
		void OnMouseMove( UINT nFlags, CPoint point );
		BOOL OnEraseBkgnd( CDC* pDC ) { return TRUE; }

		// utility functions

		// registers the window class for this control
		bool Register();
		// updates the size for the scrollbars
		void SetupScrollBars();
		// returns the location on the grid where a particular screen coordinate is
		CGridLocation GetCellsAtPoint( CPoint point );
		// returns the area that a particular call occupies
		void GetCellRect( CGridLocation location, CRect& area );
		// resize a column to its "preferred" size, by asking its helper how big the data is
		void AutoSizeColumn( int col );
		// same as AutoSizeColumn, but for all columns
		void AutoSizeColumns();
		// this enables a user to edit the cell at a particular location
		void BeginEdit( CGridLocation location, CPoint* point );
		// this validates/finalizes the changes the user has made
		void EndEdit();
		// returns a pointer to the control the user is using to edit a cell,
		//   or NULL if no cell is being edited
		CWnd* GetCurrentControl();

		// user-called functions

		// removes all of the rows and columns fram the grid
		void Clear();
		// removes all of the rows from the grid (except the mandatory null one)
		void ClearRows();
		// adds the data specified to the grid as a new row
		void AddRow( const vector< CVariant >& newRow );
		// adds a new column to the grid, with a particular header, helper object, and default value
		void AddColumn( string header, CGridControlHelper* handler=NULL, CVariant& defaultVal=CVariant() );
		// returns whether or not the data in this grid has been modified
		bool IsModified();
		// makes the grid indicate that it has not been modified
		void ResetModified();
		// sets the grid so that it cannot be modified
		void SetReadOnly( BOOL bReadOnly=TRUE );

		// accessors
		int GetColumnCount();
		int GetRowCount();
		int GetWidth(); // in pixels

	private:
		int columns, rows; // the size of the grid
		vector< string > columnHeaders; // the labels at the top of each column
		vector< vector< CVariant > > data; // the data in the grid
		vector< vector< CVariant > > dataBackup; // a copy used to determine if the grid has been modified
		vector< int > columnWidths; // the width of each column
		vector< int > rowHeights;   // the height of each row
		int defaultRowHeight;
		int defaultColumnWidth;

		int headerHeight; // the height of the labels at the top of the columns
		int topOffset;    // pixels between the top of the control and the top of the header
		int leftOffset;   // pixels between the left of the control and the first column

		int maxHScroll, maxVScroll; // data for the scrollbars (haw far the user can scroll)
		int topRow, leftColumn;     // where the grid has been scrolled to

		bool resizing;      // is the user resizing a column?
		int  resizeColumn;  // what column is being resized?
		CPoint resizeStart; // where did the usor start resizing?
		int  originalWidth; // how wide was the column to start with?

		// the set of helpers that allow the grid to show data, and let the user edit that data
		vector< CGridControlHelper* > columnHandlers;

		bool editing;               // is a call being edited?
		CGridLocation editLocation; // what cell is being edited?
		CPoint* editClickLocation;  // where did the user click to start editing that cell

		bool readonly; // is this grid modifyable?

		// this is a dummy value
		//   it is given when the user asks for data at an invalid grid location
		CVariant trash;

		DECLARE_DYNAMIC( CGridControl );
		DECLARE_MESSAGE_MAP()
};

#endif
