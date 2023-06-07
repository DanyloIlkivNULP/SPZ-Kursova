#include "control_static_text.h"

StaticText::StaticText(HWND hParent, DWORD dwDlgItem,
	const wchar_t* wcText) : Control(hParent, dwDlgItem), m_sText(wcText)
{
	(void)SetWindowText(m_hWnd, m_sText.data());
}
StaticText::~StaticText(void) { /*Code...*/ }

const wchar_t* StaticText::GetText(void)
{ return(m_sText.data()); }

void StaticText::SetText(const std::wstring& wsText)
{ m_sText = wsText;
	(void)SetWindowText(m_hWnd, m_sText.data());
}
