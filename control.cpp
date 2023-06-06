#include "control.h"

Control::Control(HWND hParent, DWORD dwDlgItem) :
	m_hParent(hParent), m_dwDlgItem(dwDlgItem)
{
	m_hWnd = GetDlgItem
		(m_hParent, m_dwDlgItem);
}
Control::~Control(void) { /*Code...*/ }

HWND Control::GetHandle(void) const
{ return(m_hWnd); }
DWORD Control::GetItemID(void) const
{ return(m_dwDlgItem); }
