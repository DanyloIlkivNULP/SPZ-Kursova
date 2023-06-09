#include "control_combobox.h"

Combobox::Combobox(HWND hParent, DWORD dwDlgItem,
	bool bState
) :
	Control(hParent, dwDlgItem, bState)
{
	/*Code...*/
}
Combobox::~Combobox(void) { /*Code...*/ }

void Combobox::AddItemString
	(const wchar_t* wcString)
{
	SendMessage(m_hWnd,
		CB_ADDSTRING, NULL, (LPARAM)wcString
	);
	SendMessage(m_hWnd,
		CB_SETCURSEL, m_iIndex += 0x1, 0x0
	);
}
const wchar_t* Combobox::ItemString(INT ID) {
	if (!m_wsText.empty())
	{ m_wsText.clear(); }

	if(ID == -0x1)
		{ return(NULL); }
	size_t sLength = SendMessage(m_hWnd,
		CB_GETLBTEXTLEN, ID, NULL
	);
	m_wsText.reserve(sLength);
	SendMessage(m_hWnd,
		CB_GETLBTEXT, ID, (LPARAM)m_wsText.data()
	);
	return(m_wsText.data());
}
INT Combobox::SelectedItemID(void) {
	INT ID = CB_ERR;
	ID = (INT)SendMessage(m_hWnd,
		CB_GETCURSEL, NULL, NULL
	);
	return(ID);
}

INT Combobox::SelectItem(INT ID) {
	INT iResult = CB_ERR;
	iResult = SendMessage(m_hWnd,
		CB_SETCURSEL, ID, 0x0
	);
	return(iResult);
}
