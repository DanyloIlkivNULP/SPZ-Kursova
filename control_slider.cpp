#include "control_slider.h"

Slider::Slider(HWND hParent, DWORD dwDlgItem, DWORD dwPos, POINT pRange) :
	Control(hParent, dwDlgItem), m_pRange(pRange)
{
	SendMessage(m_hWnd, TBM_SETRANGE,
		(WPARAM)TRUE, (LPARAM)MAKELONG(m_pRange.x, m_pRange.y));

	SendMessage(m_hWnd, TBM_SETPAGESIZE,
		0x0, (LPARAM)0xA);

	SendMessage(
		m_hWnd, TBM_SETPOS,
		(WPARAM)TRUE, (LPARAM)(m_dwPos = dwPos)
	);
}
Slider::~Slider(void) { /*Code...*/ }

DWORD Slider::GetPos(void) {
	m_dwPos = SendMessage(
		m_hWnd, TBM_GETPOS, 0x0, 0x0
	);
	return(m_dwPos);
}

void Slider::SetPos(DWORD dwPos) {
	SendMessage(
		m_hWnd, TBM_SETPOS,
		(WPARAM)TRUE, (LPARAM)(m_dwPos = dwPos)
	);
}

DWORD Slider::GetRange(void) const
{ return(m_pRange.y - m_pRange.x); }

POINT Slider::GetRangeParam(void) const
{ return(m_pRange); }
