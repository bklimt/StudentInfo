
#include "MemoryDC.h"

CMemoryDC::CMemoryDC( CDC* newDC ) : CDC() {
	dc = newDC;
	dc->GetClipBox( clientArea );
	CreateCompatibleDC( dc );
	offscreen.CreateCompatibleBitmap( dc, clientArea.Width(), clientArea.Height() );
	originalData = SelectObject( &offscreen );
	SetWindowOrg( clientArea.left, clientArea.top );
	FillSolidRect( clientArea, dc->GetBkColor() );
}

CMemoryDC::~CMemoryDC() {
	dc->BitBlt( clientArea.left,
				clientArea.top,
				clientArea.Width(), 
				clientArea.Height(),
				this, clientArea.left, clientArea.top, SRCCOPY );
	SelectObject( originalData );
}

CMemoryDC* CMemoryDC::operator->() {
	return this;
}

CMemoryDC::operator CMemoryDC*() {
	return this;
}
