#include "framework.h"

#include "logger.h"

#include "maindlg.h"
#include "resource.h"

#include "audio_engine.h"
#include "audio_player.h"

using namespace std;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance), UNREFERENCED_PARAMETER(lpCmdLine);
	Logger::LoadLogLevel
		(Logger::LogLevel::LOG_LVL_DEBUG);
	Logger::ClearLog();

	AudioEngine audio;
	audio.CreateAudio();

	MainDlg* mainDlg = new MainDlg(MAKEINTRESOURCE(IDD_MAIN_DIALOG), audio);
	mainDlg->CreateDlg(hInstance, NULL), mainDlg->ShowDlg(nCmdShow);

	MSG msg = { 0x0 };
	while (GetMessage(&msg, NULL, 0x0, 0x0) > 0x0)
		if (!IsDialogMessage(mainDlg->GetDialogHWND(), &msg))
		{ TranslateMessage(&msg), DispatchMessage(&msg); }

	audio.DestroyAudio();

	return((int)msg.wParam);
}
