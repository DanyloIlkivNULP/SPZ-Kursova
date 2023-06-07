#pragma once
#ifndef _CONTROL_BUTTON_H_
#define _CONTROL_BUTTON_H_

#include "control.h"

class Button :
	public Control
{
	std::wstring m_sText;
public:
	Button(HWND hParent,
		DWORD dwDlgItem, const wchar_t* wcText);
	~Button(void);

	const wchar_t* GetText(void);

	void SetText
		(const std::wstring& wsText);
};

#endif // _CONTROL_STATIC_TEXT_H_
