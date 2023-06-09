#pragma once
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "framework.h"

class Control {

public:
	Control(HWND hParent,
		DWORD dwDlgItem);
	Control(HWND hParent,
		DWORD dwDlgItem, BOOL bState);
	~Control(void);

	HWND GetHandle(void) const;
	DWORD GetItemID(void) const;

	enum State {
		STATE_ENABLE = 0x1,
		STATE_DISABLE = 0x0
	};
	BOOL ChangeState(bool bSate);
protected:
	HWND m_hWnd = NULL,
		m_hParent = NULL;
	DWORD m_dwDlgItem = 0x0;
};

#endif //_CONTROL_H_