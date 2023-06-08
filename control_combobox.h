#pragma once
#ifndef _CONTROL_COMBOBOX_H_
#define _CONTROL_COMBOBOX_H_

#include "control.h"

class Combobox :
	public Control
{
	INT m_iIndex = -0x1;
	std::wstring m_wsText;
public:
	Combobox(HWND hParent,
		DWORD dwDlgItem);
	~Combobox(void);

	void AddItemString
		(const wchar_t* wcString);
	const wchar_t* ItemString(INT ID);
	INT SelectedItemID(void);
};

#endif // _CONTROL_COMBOBOX_H_
