#include "control_button.h"

Button::Button(HWND hParent, DWORD dwDlgItem,
	const wchar_t* wcText) : Control(hParent, dwDlgItem)
{
	(void)SetWindowText(m_hWnd, m_wcText);
}
Button::~Button(void) { /*Code...*/ }

const wchar_t* Button::GetText(void)
{ return(m_wcText); }

void Button::SetText(const wchar_t* wcText) {
	wcscpy_s(m_wcText, MAX_PATH, wcText);
	(void)SetWindowText(m_hWnd,
		m_wcText
	);
}
