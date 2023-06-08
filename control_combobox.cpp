#include "control_combobox.h"

Combobox::Combobox(HWND hParent, DWORD dwDlgItem) :
	Control(hParent, dwDlgItem)
{
	/*Code...*/
}
Combobox::~Combobox(void) { /*Code...*/ }

void Combobox::AddItemString
	(const wchar_t* wcString)
{
	m_iIndex += 0x1;

	SendMessage(m_hWnd,
		CB_ADDSTRING, NULL, (LPARAM)wcString
	);
	SendMessage(m_hWnd,
		CB_SETCURSEL, m_iIndex, 0x0
	);
}
const wchar_t* Combobox::ItemString(DWORD dwID) {
	if (!m_wsText.empty())
	{ m_wsText.clear(); }

	if(dwID == -0x1)
		{ return(NULL); }
	size_t sLength = SendMessage(m_hWnd,
		CB_GETLBTEXTLEN, dwID, NULL
	);
	m_wsText.reserve(sLength);
	SendMessage(m_hWnd,
		CB_GETLBTEXT, dwID, (LPARAM)m_wsText.data()
	);
	return(m_wsText.data());
}
DWORD Combobox::SelectedItemID(void) {
	return((DWORD)SendMessage(m_hWnd,
		CB_GETCURSEL, NULL, NULL)
	);
}
