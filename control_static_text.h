#pragma once
#ifndef _CONTROL_STATIC_TEXT_H_
#define _CONTROL_STATIC_TEXT_H_

#include "control.h"

class StaticText :
	public Control
{
	wchar_t m_wcText[256]{};
public:
	StaticText(HWND hParent,
		DWORD dwDlgItem, const wchar_t* wcText);
	~StaticText(void);

	const wchar_t* GetText(void);

	void SetText
		(const wchar_t* wcText);
};

#endif // _CONTROL_STATIC_TEXT_H_
