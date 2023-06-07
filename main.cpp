#include "framework.h"

#include "logger.h"

#include "maindlg.h"
#include "resource.h"

#include "audio_engine.h"

using namespace std;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	INITCOMMONCONTROLSEX icc = {
		sizeof(icc), ICC_WIN95_CLASSES
	};
	if (!InitCommonControlsEx(&icc))
	{ Logger::ShowLastError
		(L"InitCommonControlsEx(&icc)"); return(-0x1);
	}

	LPWSTR* szArglist;
	signed int nArgs;

	szArglist = CommandLineToArgvW
		(GetCommandLineW(), &nArgs);

	wchar_t* wcWavFile = NULL;
	if (szArglist == NULL)
	{ Logger::ShowLastError
		(L"CommandLineToArgvW - Failed!"); return(-0x1);
	} else if (nArgs > 0x1)
	{ wcWavFile = szArglist[0x1]; }

	Logger::LoadLogLevel
		(Logger::LogLevel::LOG_LVL_DEBUG);
	Logger::ClearLog();

	AudioEngine audio;
	audio.CreateAudio();

	MainDlg* mainDlg = new
		MainDlg(MAKEINTRESOURCE(IDD_MAIN_DIALOG), wcWavFile, audio);
	mainDlg->CreateDlg(hInstance, NULL), mainDlg->ShowDlg(nCmdShow);

	MSG msg = { 0x0 };
	while (GetMessage(&msg, NULL, 0x0, 0x0) > 0x0)
		if (!IsDialogMessage(mainDlg->GetDialogHWND(), &msg))
		{ TranslateMessage(&msg), DispatchMessage(&msg); }

	audio.DestroyAudio();

	LocalFree(szArglist);
	return((int)msg.wParam);
}
