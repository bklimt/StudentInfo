
/*
 * StudentInfo.h
 *
 * This class is the application itself.  It is derived
 *   from CWinApp like any MFC application
 *
 */

#include <afxwin.h> // for MFC

class CStudentInfo : public CWinApp {
	public:
		// called when the application is run
		//   InitInstance() is equivalent to main()
		BOOL InitInstance();
};
