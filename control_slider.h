#pragma once
#ifndef _CONTROL_SLIDER_H_
#define _CONTROL_SLIDER_H_

#include "control.h"

class Slider :
	public Control
{
	DWORD m_dwPos = 0x0;
	POINT m_pRange = { 0x0 };
public:
	Slider(HWND hParent,
		DWORD dwDlgItem, DWORD dwPos, POINT pRange);
	~Slider(void);

	DWORD GetPos(void);

	void SetPos
		(DWORD dwPos);
};

#endif // _CONTROL_SLIDER_H_
