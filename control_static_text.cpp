#include "control_static_text.h"

StaticText::StaticText(HWND hParent, DWORD dwDlgItem,
	const wchar_t* wcText) : Control(hParent, dwDlgItem)
{
	(void)wcscpy_s(m_wcText, MAX_PATH, wcText);
	(void)SetWindowText(m_hWnd,
		m_wcText
	);
}
StaticText::~StaticText(void) { /*Code...*/ }

const wchar_t* StaticText::GetText(void)
{ return(m_wcText); }

void StaticText::SetText(const wchar_t* wcText) {
	if (wcslen(m_wcText) != NULL)
	{ ZeroMemory(m_wcText,
		sizeof(wchar_t) * MAX_PATH);
	}

	wcscpy_s(m_wcText, MAX_PATH, wcText);
	(void)SetWindowText(m_hWnd,
		m_wcText
	);
}
