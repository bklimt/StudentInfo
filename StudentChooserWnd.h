
#include <afxwin.h>
#include "StudentChooserDlg.h"

class CStudentChooserWnd : public CMDIChildWnd {
	public:
		BOOL Create( CMDIFrameWnd* parent );
		void OnClose();
		//void OnPaint();
		void OnSetFocus( CWnd* pOldWnd );
		void OnSize( UINT nType, int cx, int cy );
		BOOL OnEraseBkgnd( CDC* pDC );

		void RefreshStudentList();
		void NewStudent();
		void CreateStudent();
		void OpenStudent();
		void DeleteStudent();

		int GetSchoolYearID();
	private:
		CStudentChooserDlg dialog;
		DECLARE_DYNAMIC( CStudentChooserWnd )
		DECLARE_MESSAGE_MAP()
};

#ifdef DECLARE_CHOOSER
	CStudentChooserWnd* chooser;
#else
	extern CStudentChooserWnd* chooser;
#endif