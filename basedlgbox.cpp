#include "basedlgbox.h"

#include "logger.h"

BaseDlgBox::BaseDlgBox(LPWSTR dlgResName) : 
		m_dlgResName(dlgResName) { /*Code...*/ }
BaseDlgBox::~BaseDlgBox(void) {
	//Code...
}
HWND BaseDlgBox::GetDialogHWND(void) const { return(m_hWnd); }
BOOL BaseDlgBox::CreateDlg
	(HINSTANCE hInstance, HWND hParent)
{
	BOOL bResult = 0x1;

	if(!m_hInstance && !m_hParent)
	{ m_hInstance = hInstance;
		m_hParent = hParent;
	}
	else { bResult = 0x0; goto linkExit; }

	m_hWnd = CreateDialog(m_hInstance, m_dlgResName, m_hParent,
		reinterpret_cast<DLGPROC>(WindowProc));
	if (!m_hWnd) { bResult = 0x0; goto linkExit; }

	SetWindowLongPtr(m_hWnd, GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(this));

	if(!OnUserCreate()) { (void)Logger::LogMessage
		(Logger::LogLevel::LOG_LVL_INFO,
			L"[Function] : OnUserCreate" L" - Success!\t[File] : " __FILE__);
	}
	else { (void)Logger::LogMessage
		(Logger::LogLevel::LOG_LVL_INFO,
			L"[Function] : OnUserCreate" L" - Failed!\t[File] : " __FILE__);
	}

linkExit:
	return(bResult);
}
BOOL BaseDlgBox::DestroyDlg(int nExitCode) {
	BOOL bResult = 0x1;
	if (!m_hWnd)
	{ bResult = 0x0;
		goto linkExit;
	}

	bResult = EndDialog(m_hWnd, nExitCode);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(nullptr));
	m_hWnd = NULL;

	if(!OnUserDestroy()) { (void)Logger::LogMessage
		(Logger::LogLevel::LOG_LVL_INFO,
			L"[Function] : OnUserDestroy" L" - Success!\t[File] : " __FILE__);
	}
	else { (void)Logger::LogMessage
		(Logger::LogLevel::LOG_LVL_INFO,
			L"[Function] : OnUserDestroy" L" - Failed!\t[File] : " __FILE__);
	}

linkExit:
	return(bResult);
}
BOOL BaseDlgBox::ShowDlg(int nCmdShow) {
	BOOL bResult = ShowWindow
		(m_hWnd, nCmdShow);
	return(bResult);
}
LRESULT CALLBACK BaseDlgBox::WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	BaseDlgBox* pBaseDlgBox = nullptr;

	pBaseDlgBox = reinterpret_cast<BaseDlgBox*>
		(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (uMsg == WM_INITDIALOG) { return(0x1); }

	if (pBaseDlgBox)
	{ return(pBaseDlgBox->HandleMessage(uMsg, wParam, lParam)); }
	else { return(0x0); }
}
LRESULT CALLBACK BaseDlgBox::HandleMessage(UINT _In_ uMsg,
	WPARAM _In_ wParam, LPARAM _In_ lParam) {
	switch (uMsg) {
	
	case WM_CLOSE:
		EndDialog(m_hWnd, wParam); m_hWnd = NULL;
		return(TRUE);

	default:
		break;
	}
	return(0x0);
}
