#pragma once
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "framework.h"

class Control {

public:
	Control(HWND hParent,
		DWORD dwDlgItem);
	~Control(void);

	HWND GetHandle(void) const;
	DWORD GetItemID(void) const;
protected:
	HWND m_hWnd = NULL,
		m_hParent = NULL;
	DWORD m_dwDlgItem = 0x0;
};

#endif //_CONTROL_H_