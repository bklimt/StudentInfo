
#include <afxwin.h>
#include <string>
#include "StudentEditDlg.h"
using namespace std;

class CStudentEditWnd : public CMDIChildWnd {
	public:
		BOOL Create( int schoolYearID, int x, int y, CMDIFrameWnd* parent );
		BOOL Create( string ssn, int schoolYearID, int x, int y, CMDIFrameWnd* parent );
		void OnClose();
		//void OnPaint();
		void OnSetFocus( CWnd* pOldWnd );
		void OnSize( UINT nType, int cx, int cy );
		BOOL OnEraseBkgnd( CDC* pDC );

		void SaveStudent();
		bool AttemptClose();
		bool IsModified();

	private:
		CStudentEditDlg dialog;
		DECLARE_DYNAMIC( CStudentEditWnd )
		DECLARE_MESSAGE_MAP()
};
