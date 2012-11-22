
#ifndef MEMORYDC_H
#define MEMORYDC_H

#include <afxwin.h>

class CMemoryDC : public CDC {
	public:
		CMemoryDC( CDC* pDC );
		~CMemoryDC();
		CMemoryDC* operator->();
		operator CMemoryDC*();
	private:
		CBitmap  offscreen;
		CBitmap* originalData;
		CDC*     dc;
		CRect    clientArea;
};

#endif
