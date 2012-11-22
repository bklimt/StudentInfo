
#ifndef FANCYCHECKBOX_H
#define FANCYCHECKBOX_H

#include <afxwin.h>

class CFancyCheckBox : public CButton {
	public:
		BOOL PreTranslateMessage( MSG* pMsg );
		bool GetValue();
		void SetValue( bool newValue );
		bool IsModified();
		void ResetModified();
	private:
		bool oldValue;
		DECLARE_DYNAMIC( CFancyCheckBox )
};

#endif