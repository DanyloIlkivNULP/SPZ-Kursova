#include "control_button.h"

Button::Button(HWND hParent, DWORD dwDlgItem,
	const wchar_t* wcText, bool bState) : Control(hParent, dwDlgItem, bState)
{
	if (wcText == NULL)
	{ wcText = (wchar_t*)L'\0'; }
	wcscpy_s(m_wcText, MAX_PATH, wcText);
	(void)SetWindowText(m_hWnd, m_wcText);
}
Button::~Button(void) { /*Code...*/ }

const wchar_t* Button::GetText(void)
{ return(m_wcText); }

void Button::SetText(const wchar_t* wcText) {
	if (wcscmp(m_wcText, wcText) == NULL)
		{ return; }
	if (wcslen(m_wcText) != NULL)
	{ ZeroMemory(m_wcText,
		sizeof(wchar_t) * MAX_PATH);
	}

	if (wcText == NULL)
	{ wcText = (wchar_t*)L'\0'; }

	wcscpy_s(m_wcText, MAX_PATH, wcText);
	(void)SetWindowText(m_hWnd,
		m_wcText
	);
}
