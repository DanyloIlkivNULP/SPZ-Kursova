#ifndef _BASEDLGBOX_
#define _BASEDLGBOX_

#include "framework.h"

class BaseDlgBox {
	LPWSTR m_dlgResName = NULL;
	HWND m_hParent = NULL;

public:
	BaseDlgBox(LPWSTR dlgResName);
	virtual ~BaseDlgBox(void);

	HWND GetDialogHWND(void) const;

	virtual BOOL CreateDlg
		(HINSTANCE hInstance, HWND hParent);
	virtual BOOL DestroyDlg
		(int nExitCode);
	virtual BOOL ShowDlg(int nCmdShow);

	virtual bool
		OnUserCreate(void) = 0x0;
	virtual bool
		OnUserDestroy(void) = 0x0;

private:
	static LRESULT CALLBACK WindowProc(HWND _In_ hWnd, UINT _In_ uMsg, 
		WPARAM _In_ wParam, LPARAM _In_ lParam);
protected:
	HINSTANCE m_hInstance = NULL;

	HWND m_hWnd = NULL;

	virtual LRESULT CALLBACK HandleMessage(UINT _In_ uMsg,
		WPARAM _In_ wParam, LPARAM _In_ lParam);
};

#endif //_BASEDLGBOX_
