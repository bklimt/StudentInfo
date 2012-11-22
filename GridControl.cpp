
/* 
 * GridControl.h
 *
 * This class is a grid that resembles a spreadsheet.
 *   It is similar to Java's JTable or MacOS X's NSTableView?.
 *   I don't know why it isn't part of Windows.
 *
 */

#include "GridControl.h"

#define MIN(x,y) ( ( (x) < (y) ) ? (x) : (y) )
#define MAX(x,y) ( ( (x) > (y) ) ? (x) : (y) )
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

CGridControl::CGridControl( bool isReadOnly, int rowHeight, int columnWidth ) {
	if ( !Register() ) {
		MessageBox( "Unable to register grid control class!", "" );
		PostQuitMessage( -1 );
	}
	readonly = isReadOnly;
	resizing = false;
	rows = 1;
	columns = 0;
	headerHeight = defaultRowHeight = rowHeight;
	rowHeights.push_back( defaultRowHeight );
	defaultColumnWidth = columnWidth;
	topRow = leftColumn = 0;
	maxHScroll = maxVScroll = 0;
	topOffset = leftOffset = 2;
	data.push_back( vector<CVariant>() );
	dataBackup = data;
	// editing stuff
	editing = false;
	editClickLocation = NULL;
}

CGridControl::~CGridControl() {
	for ( int i=0; i<columnHandlers.size(); i++ ) {
		delete columnHandlers[i];
	}
}

//   returns the data at a particular location
CVariant& CGridControl::operator[]( CGridLocation index ) {
	if ( index.IsValid( rows, columns ) ) {
		return data[ index.GetRow() ][ index.GetColumn() ];
	}
	trash = CVariant();
	return trash;
}

// needed to handle tab and enter keys correctly
BOOL CGridControl::PreTranslateMessage( MSG* pMsg ) {
	if ( pMsg->message == WM_KEYDOWN && (char)(pMsg->wParam) == '\t' && editing ) {
		int row = editLocation.GetRow();
		int column = editLocation.GetColumn();
		if ( GetKeyState( VK_SHIFT ) & (1<<31) ) {
			EndEdit();
			if ( column == 0 ) {
				if ( row == 0 ) {
					return CWnd::PreTranslateMessage( pMsg );
				} else {
					CRect cellArea, totalArea;
					GetCellRect( CGridLocation( row-1, 0 ), cellArea );
					GetClientRect( totalArea );
					while ( cellArea.top < totalArea.top+headerHeight ) {
						topRow--;
						SetScrollPos( SB_VERT, topRow );
						Invalidate( FALSE );
						OnPaint();
						GetCellRect( CGridLocation( row-1, 0 ), cellArea );
						GetClientRect( totalArea );
					}
					BeginEdit( CGridLocation( row-1, GetColumnCount()-1 ), NULL );
				}
			} else {
				BeginEdit( CGridLocation( row, column-1 ), NULL );
			}
		} else {
			int oldRows = GetRowCount();
			bool rowRemoved;
			//EndEdit();
			if ( GetRowCount() != oldRows ) {
				rowRemoved = true;
			} else {
				rowRemoved = false;
			}
			if ( column == GetColumnCount()-1 ) {
				if ( row == GetRowCount()-1 ) {
					//EndEdit();
					topRow = leftColumn = 0;
					SetScrollPos( SB_VERT, topRow );
					SetScrollPos( SB_HORZ, leftColumn );
					Invalidate( FALSE );
					return CWnd::PreTranslateMessage( pMsg );
					//PostMessage( pMsg->message, (WPARAM)'\t', pMsg->lParam );
				} else {
					if ( rowRemoved ) {
						BeginEdit( CGridLocation( row,   0 ), NULL );
					} else {
						CRect cellArea, totalArea;
						GetCellRect( CGridLocation( row+1, 0 ), cellArea );
						GetClientRect( totalArea );

						while ( cellArea.bottom > totalArea.bottom ) {
							topRow++;
							SetScrollPos( SB_VERT, topRow );
							Invalidate( FALSE );
							OnPaint();
							GetCellRect( CGridLocation( row+1, 0 ), cellArea );
							GetClientRect( totalArea );
						}

						BeginEdit( CGridLocation( row+1, 0 ), NULL );
					}
				}
			} else {
				BeginEdit( CGridLocation( row, column+1 ), NULL );
			}
		}
		return TRUE;
	} else {
		return CWnd::PreTranslateMessage( pMsg );
	}
}

// event handlers
//   these define how the control behaves

void CGridControl::OnPaint() {
	int i, j;
	CRect client;
	CPaintDC realDC( (CWnd*) this );
	//CPaintDC* dc = &realDC;
	{
		CMemoryDC dc( &realDC );

		GetClientRect( client );
		CBrush workspaceBrush( GetSysColor( COLOR_APPWORKSPACE ) );
		CBrush buttonBrush( GetSysColor( COLOR_BTNFACE ) );
		//CBrush disabledBrush( GetSysColor( CTLCOLOR_DLG ) );
		CFont* font = CFont::FromHandle( (HFONT)GetStockObject( DEFAULT_GUI_FONT ) );
		CFont* oldFont = (CFont*) dc->SelectObject( font );

		if ( !maxHScroll ) {
			leftColumn = 0;
		}

		if ( !maxVScroll ) {
			topRow = 0;
		}

		dc->FillRect( client, &workspaceBrush );
		if ( readonly ) {
			//dc->SelectObject( disabledBrush );
		}
		if ( columns > 0 ) {
			int left = leftOffset;
			for ( j=leftColumn; j<columns; j++ ) {
				if ( left > client.Width() ) {
						break;
				}
				//columnHelpers.push_back( GetColumnHelper(j) );
				//dc->Rectangle( left, topOffset, left+columnWidths[j], topOffset+headerHeight );
				dc->FillRect( CRect( left, topOffset, left+columnWidths[j], topOffset+headerHeight ), &buttonBrush );
				dc->DrawEdge( CRect( left, topOffset, left+columnWidths[j], topOffset+headerHeight ), EDGE_RAISED, BF_RECT );
				dc->SetBkMode( TRANSPARENT );
				dc->DrawText(
					(CString)( columnHeaders[j].c_str() ), 
					CRect( left+2, topOffset, left+columnWidths[j], topOffset+headerHeight ), 
					DT_VCENTER | DT_SINGLELINE
				);
				left += columnWidths[j];
			}
			if ( rows > 0 ) {
				int top = headerHeight + topOffset;
				for ( i=topRow; i<rows; i++ ) {
					left = leftOffset;
					if  ( top > client.Height() ) {
						break;
					}
					for ( j=leftColumn; j<columns; j++ ) {
						if ( left > client.Width() ) {
							break;
						}
						CGridControlHelper* handler = columnHandlers[ j-leftColumn ];
						if ( handler ) {
							CRect area( left-1, top-1, left+columnWidths[j], top+rowHeights[i] );
							handler->OnPaint( dc, area, data[i][j] );
						}
						left += columnWidths[j];
					}
					top += rowHeights[i];
				}
			}
		}
		dc->DrawEdge( client, EDGE_SUNKEN, BF_TOPLEFT | BF_RIGHT );
		dc->SelectObject( oldFont );
		SetupScrollBars();
	}
}

void CGridControl::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ) {
	EndEdit();

	int scrollPos = GetScrollPos( SB_HORZ );
	RECT client;
	GetClientRect( &client );

	int newPos = scrollPos;

	switch (nSBCode) {
		case SB_LINERIGHT     : { newPos = MIN( scrollPos+1, maxHScroll ); } break;
		case SB_LINELEFT      : { newPos = MAX( scrollPos-1, 0          ); } break;
		case SB_PAGERIGHT     : { newPos = MIN( scrollPos+1, maxHScroll ); } break;
		case SB_PAGELEFT      : { newPos = MAX( scrollPos-1, 0          ); } break;
		case SB_RIGHT         : { newPos = maxHScroll                    ; } break;
		case SB_LEFT          : { newPos = 0                             ; } break;
		case SB_THUMBPOSITION :
		case SB_THUMBTRACK    : { newPos = nPos                          ; } break;
		default : break;
	}

	leftColumn = newPos;
	SetScrollPos( SB_HORZ, newPos );
	Invalidate( FALSE );
}

void CGridControl::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ) {
	EndEdit();
	
	int scrollPos = GetScrollPos( SB_VERT );
	RECT client;
	GetClientRect( &client );

	int newPos = scrollPos;

	switch (nSBCode) {
		case SB_LINEDOWN      : { newPos = MIN( scrollPos+1, maxVScroll ); } break;
		case SB_LINEUP        : { newPos = MAX( scrollPos-1, 0          ); } break;
		case SB_PAGEDOWN      : { newPos = MIN( scrollPos+1, maxVScroll ); } break;
		case SB_PAGEUP        : { newPos = MAX( scrollPos-1, 0          ); } break;
		case SB_BOTTOM        : { newPos = maxVScroll                    ; } break;
		case SB_TOP           : { newPos = 0                             ; } break;
		case SB_THUMBPOSITION :
		case SB_THUMBTRACK    : { newPos = nPos                          ; } break;
		default : break;
	}

	topRow = newPos;
	SetScrollPos( SB_VERT, newPos );
	Invalidate( FALSE );
}

void CGridControl::OnLButtonDown( UINT nFlags, CPoint point ) {
	EndEdit();
	CGridLocation location = GetCellsAtPoint( point );
	if ( location.IsColumnResizeArea() ) {
		resizing = true;
		resizeStart = point;
		resizeColumn = location.GetColumn();
		originalWidth = columnWidths[ resizeColumn ];
	} else {
		if ( location.IsCell() ) {
			BeginEdit( location, &point );
		}
	}
	CWnd::OnLButtonDown( nFlags, point );
}

void CGridControl::OnLButtonUp( UINT nFlags, CPoint point ) {
	resizing = false;
	CWnd::OnLButtonUp( nFlags, point );
	OnMouseMove( nFlags, point );
}

void CGridControl::OnLButtonDblClk( UINT nFlags, CPoint point ) {
	CGridLocation location = GetCellsAtPoint( point );
	if ( location.IsColumnResizeArea() ) {
		AutoSizeColumn( location.GetColumn() );
	}
	CWnd::OnLButtonDblClk( nFlags, point );
}

void CGridControl::OnMouseMove( UINT nFlags, CPoint point ) {
	if ( resizing && !KEY_DOWN( VK_LBUTTON ) ) {
		resizing = false;
	}
	if ( resizing ) {
		int newColumnWidth = originalWidth + ( point.x - resizeStart.x );
		if ( newColumnWidth > 5 ) {
			columnWidths[ resizeColumn ] = newColumnWidth;
		}
		Invalidate( FALSE );
	} else {
		CGridLocation location = GetCellsAtPoint( point );
		if ( location.IsColumnResizeArea() ) {
			SetCursor( AfxGetApp()->LoadStandardCursor( IDC_SIZEWE ) );
		} else {
			SetCursor( AfxGetApp()->LoadStandardCursor( IDC_ARROW ) );
		}
	}
	CWnd::OnMouseMove( nFlags, point );
}

// registers the window class for this control
bool CGridControl::Register() {
	// standard windows stuff
	WNDCLASS wc;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if ( !GetClassInfo(hInst, "CGridControl", &wc ) ) {
		wc.style            = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wc.lpfnWndProc      = ::DefWindowProc;
		wc.cbClsExtra       = 0;
		wc.cbWndExtra       = 0;
		wc.hInstance        = hInst;
		wc.hIcon            = (HICON) GetStockObject( (int)IDI_APPLICATION );
		wc.hCursor          = (HCURSOR) GetStockObject( (int)IDC_ARROW );
		wc.hbrBackground    = (HBRUSH) ( COLOR_APPWORKSPACE );
		wc.lpszMenuName     = NULL;
		wc.lpszClassName    = "CGridControl";

		if ( !AfxRegisterClass(&wc) ) {
			return false;
		}
	}

	return true;
}

// updates the size for the scrollbars
void CGridControl::SetupScrollBars() {
	int i, size;
	RECT client;
	GetClientRect( &client );
	if ( data.size() > 0 && data[0].size() > 0 ) {
		// vertical
		SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask = SIF_RANGE;
		si.nMin = 0;
		size = headerHeight + topOffset;
		maxVScroll = data.size()-1;
		for ( i=data.size()-1; i>=0; i-- ) {
			size += rowHeights[i];
			if ( size >= ( client.bottom - client.top ) - headerHeight ) {
				break;
			}
			maxVScroll = i;
		}
		si.nMax = maxVScroll;
		SetScrollInfo( SB_VERT, &si, TRUE );

		// horizontal
		size = leftOffset;
		maxHScroll = data[0].size()-1;
		for ( i=data[0].size()-1; i>=0; i-- ) {
			size += columnWidths[i];
			if ( size >= ( client.right - client.left ) ) {
				break;
			}
			maxHScroll = i;
		}
		si.nMax = maxHScroll;
		SetScrollInfo( SB_HORZ, &si, TRUE );
	}
}

// returns the location on the grid where a particular screen coordinate is
CGridLocation CGridControl::GetCellsAtPoint( CPoint point ) {
	int i;
	CRect client;
	CGridLocation location;
	GetClientRect( client );

	int left = leftOffset;
	int top = topOffset + headerHeight;

	if ( columns == 0 ) return location;

	for ( i=leftColumn; i<columns; i++ ) {
		if ( left > client.Width() ) return location;
		left += columnWidths[i];

		if ( left - point.x > 2 ) {
			location.SetColumn( i );
			break;
		}
		if ( abs( point.x - left ) <= 2 ) {
			location.SetColumnBorder( i );
			break;
		}
	}
	if ( point.y == top ) {
		location.SetOnHeaderBorder();
		return location;
	}
	if ( point.y < top ) {
		location.SetOnHeader();
		return location;
	}
	for ( i=topRow; i<rows; i++ ) {
		if  ( top > client.Height() ) return CGridLocation();
		top += rowHeights[i];

		if ( point.y < top ) {
			location.SetRow( i );
			return location;
		}
		if ( point.y == top ) {
			location.SetRowBorder( i );
			return location;
		}
	}
	return location;
}

// returns the area that a particular call occupies
void CGridControl::GetCellRect( CGridLocation location, CRect& area ) {
	area = CRect( 0, 0, 0, 0 );
	if ( !location.IsCell() ) return;
	int top = topOffset + headerHeight;
	int left = leftOffset;
	if ( location.GetRow() < topRow || location.GetColumn() < leftColumn ) return;
	for ( int i=topRow; i<rows; i++ ) {
		left = leftOffset;
		for ( int j=leftColumn; j<columns; j++ ) {
			if ( i == location.GetRow() && j == location.GetColumn() ) {
				area = CRect( left-1, top-1, left+columnWidths[j], top+rowHeights[i] );
			}
			left += columnWidths[j];
		}
		top += rowHeights[i];
	}
}

// resize a column to its "preferred" size, by asking its helper how big the data is
void CGridControl::AutoSizeColumn( int col ) {
	EndEdit();
	CDC* dc = GetDC();
	if ( dc ) {
		CFont* font = CFont::FromHandle( (HFONT)GetStockObject( DEFAULT_GUI_FONT ) );
		CFont* oldFont = (CFont*) dc->SelectObject( font );

		int maxWidth = 5;

		CString str = (LPCTSTR)( columnHeaders[col].c_str() );
		CSize size = dc->GetTextExtent( str );
		size.cx += 10;
		if ( size.cx > maxWidth ) {
			maxWidth = size.cx;
		}

		for ( int i=0; i<rows; i++ ) {
			int rowWidth = columnHandlers[ col ]->GetPreferredWidth( dc, data[i][col] );
			maxWidth = ( rowWidth > maxWidth ? rowWidth : maxWidth );
		}
		columnWidths[col] = maxWidth;
		Invalidate( FALSE );

		dc->SelectObject( oldFont );
	}
}

// same as AutoSizeColumn, but for all columns
void CGridControl::AutoSizeColumns() {
	EndEdit();
	// loop through all of the columns
	int totalWidth = 0;
	for ( int i=0; i<GetColumnCount(); i++ ) {
		AutoSizeColumn( i );
		totalWidth += columnWidths[ i ];
	}
	if ( totalWidth < ( GetWidth()-20 ) ) {
		double ratio = (double)( GetWidth()-20 ) / (double)totalWidth;
		for ( int j=0; j<GetColumnCount(); j++ ) {
			columnWidths[ j ] = (int)( (double)columnWidths[ j ] * ratio );
		}
	}
}

// this enables a user to edit the cell at a particular location
void CGridControl::BeginEdit( CGridLocation location, CPoint* point ) {
	if ( !readonly ) {
		CRect area;
		EndEdit();
		editLocation = location;
		editClickLocation = point;
		GetCellRect( location, area );

		CRect totalArea;
		GetClientRect( totalArea );
		while ( area.top < totalArea.top+headerHeight ) {
			topRow--;
			SetScrollPos( SB_VERT, topRow );
			Invalidate( FALSE );
			OnPaint();
			GetCellRect( location, area );
			GetClientRect( totalArea );
		}
		while ( area.bottom > totalArea.bottom ) {
			topRow++;
			SetScrollPos( SB_VERT, topRow );
			Invalidate( FALSE );
			OnPaint();
			GetCellRect( location, area );
			GetClientRect( totalArea );
		}

		columnHandlers[ editLocation.GetColumn() ]->OnBeginEdit( area, (*this)[ editLocation ], point, this );
		editing = true;
	}
}

// this validates/finalizes the changes the user has made
void CGridControl::EndEdit() {
	if ( editing ) {
		editing = false;
		columnHandlers[ editLocation.GetColumn() ]->OnEndEdit( (*this)[ editLocation ] );

		bool isNull = true;
		for ( int i=0; i<GetColumnCount() && isNull; i++ ) {
			CVariant cellData = ( (*this)[ CGridLocation( editLocation.GetRow(), i ) ] );
			if ( !columnHandlers[ i ]->IsNull( cellData ) ) {
				isNull = false;
			}
		}

		if ( editLocation.GetRow() == ( GetRowCount() - 1 ) ) {
			if ( !isNull ) {
				rowHeights.push_back( defaultRowHeight );
				data.push_back( vector<CVariant>( GetColumnCount(), CVariant() ) );
				rows++;
				Invalidate( FALSE );
			}
		} else {
			if ( isNull ) {
				vector< vector< CVariant > > newData;
				vector< int > newRowHeights;
				for ( int i=0; i<GetRowCount(); i++ ) {
					if ( i != editLocation.GetRow() ) {
						newData.push_back( data[i] );
						newRowHeights.push_back( rowHeights[i] );
					}
				}
				data = newData;
				rowHeights = newRowHeights;
				rows--;
				Invalidate( FALSE );
			}
		}
		editClickLocation = NULL;
	}
}

// returns a pointer to the control the user is using to edit a cell,
//   or NULL if no cell is being edited
CWnd* CGridControl::GetCurrentControl() {
	if ( editing ) {
		CWnd* child = columnHandlers[ editLocation.GetColumn() ]->GetControl();
		if ( child ) {
			return child;
		} else {
			return this;
		}
	} else {
		return this;
	}
}

// removes all of the rows and columns fram the grid
void CGridControl::Clear() {
	rows = 1;
	columns = 0;
	data.clear();
	data.push_back( vector<CVariant>() );
	columnHeaders.clear();
	columnHandlers.clear();
	columnWidths.clear();
	rowHeights.clear();
	rowHeights.push_back( defaultRowHeight );
	topRow = leftColumn = 0;
	dataBackup = data;
}


// removes all of the rows from the grid (except the mandatory null one)
void CGridControl::ClearRows() {
	// clear the old data from the table
	data.clear();
	rowHeights.clear();
	// add a new blank row for input
	rows = 1;
	data.push_back( vector< CVariant >( GetColumnCount(), CVariant() ) );
	rowHeights.push_back( defaultRowHeight );
	// reset the view
	topRow = 0;
	dataBackup = data;
}

// adds the data specified to the grid as a new row
void CGridControl::AddRow( const vector< CVariant >& newRow ) {
	if ( newRow.size() == columns && columns != 0 ) {
		for ( int i=0; i<GetColumnCount(); i++ ) {
			data[data.size()-1][i] = newRow[i];
		}
		rowHeights.push_back( defaultRowHeight );
		data.push_back( vector<CVariant>( GetColumnCount(), CVariant() ) );
		rows++;
	}
	dataBackup = data;
}

// adds a new column to the grid, with a particular header, helper object, and default value
void CGridControl::AddColumn( string header, CGridControlHelper* handler, CVariant& defaultVal ) {
	columnHeaders.push_back( header );
	if ( handler ) {
		columnHandlers.push_back( handler );
	} else {
		columnHandlers.push_back( new CGridControlHelper() );
	}
	columnWidths.push_back( defaultColumnWidth );
	for ( int i=0; i<data.size(); i++ ) {
		data[i].push_back( defaultVal );
	}
	columns++;
	dataBackup = data;
}

// returns whether or not the data in this grid has been modified
bool CGridControl::IsModified() {
	if ( data.size() != dataBackup.size() ) {
		return true;
	}

	for ( int i=0; i<data.size(); i++ ) {
		if ( data[i].size() != dataBackup[i].size() ) {
			return true;
		}

		for ( int j=0; j<data[i].size(); j++ ) {
			if ( editing && editLocation.GetRow() == i && editLocation.GetColumn() == j ) {
				if ( columnHandlers[ editLocation.GetColumn() ]->IsModifiedFrom( dataBackup[i][j] ) ) {
					return true;
				}
			} else {
				if ( data[i][j] != dataBackup[i][j] ) {
					return true;
				}
			}
		}
	}

	return false;
}

// makes the grid indicate that it has not been modified
void CGridControl::ResetModified() {
	EndEdit();
	dataBackup = data;
}

// sets the grid so that it cannot be modified
void CGridControl::SetReadOnly( BOOL bReadOnly ) {
	readonly = ( bReadOnly ? true : false );
}

// accessors

int CGridControl::GetColumnCount() { return columns; }
int CGridControl::GetRowCount() { return rows; }
int CGridControl::GetWidth() {
	RECT area;
	GetClientRect( &area );
	return ( area.right - area.left );
}

IMPLEMENT_DYNAMIC( CGridControl, CWnd )

BEGIN_MESSAGE_MAP( CGridControl, CWnd )
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
