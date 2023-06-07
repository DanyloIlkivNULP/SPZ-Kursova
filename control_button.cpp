#include "control_button.h"

Button::Button(HWND hParent, DWORD dwDlgItem,
	const wchar_t* wcText) : Control(hParent, dwDlgItem), m_sText(wcText)
{
	(void)SetWindowText(m_hWnd, m_sText.data());
}
Button::~Button(void) { /*Code...*/ }

const wchar_t* Button::GetText(void)
{ return(m_sText.data()); }

void Button::SetText(const std::wstring& wsText) {
	m_sText = wsText;
	(void)SetWindowText(m_hWnd, m_sText.data());
}
