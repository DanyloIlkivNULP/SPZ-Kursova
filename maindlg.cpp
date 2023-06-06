#include "maindlg.h"
#include "resource.h"

#include "audio_engine.h"
#include "audio_player.h"

#include "logger.h"

namespace sample {
	const wchar_t* cSample[] = {
			L"test_0.wav",
			L"test_1.wav",
			L"test_2.wav"
	};
	const size_t sCnt = sizeof(cSample) /
		sizeof(const wchar_t*);

	AUDIOID nSample[sCnt] = { 0x0 };
};

MainDlg::MainDlg(LPWSTR dlgResName, AudioEngine& refAE) : m_refAE(refAE),
	BaseDlgBox(dlgResName)
{
	for (int i = 0x0; i < sizeof(sample::cSample) / sizeof(wchar_t*); i++) {
		sample::nSample[i] = m_refAE.LoadAudioSample(sample::cSample[i]);
		if (sample::nSample[i] == -(0x1))
		{ m_refAE.DestroyAudio(); }
	}

	AUDIOID nMusic = m_refAE.
		LoadAudioSample(L"resurrection.wav");
	if (nMusic == -(0x1))
	{ m_refAE.DestroyAudio(); }

	m_ap = std::make_unique
		<AudioPlayer>(&m_refAE, nMusic);
}
MainDlg::~MainDlg(void)
{ /*Code...*/ }

bool MainDlg::OnUserCreate(void) {
	m_conTrackBar.hWndTrack = GetDlgItem
		(m_hWnd, IDC_SLIDER);

	SendMessage(m_conTrackBar.hWndTrack, TBM_SETRANGE,
		(WPARAM)TRUE, (LPARAM)MAKELONG(0x0, 100));

	SendMessage(m_conTrackBar.hWndTrack, TBM_SETSEL,
		0x0, (LPARAM)MAKELONG(0x0, 0x0));

	SendMessage(m_conTrackBar.hWndTrack, TBM_SETPAGESIZE,
		0x0, (LPARAM)32);

	return(true);
}
bool MainDlg::OnUserDestroy(void) {
	/*Code...*/
	return(true);
}

LRESULT CALLBACK MainDlg::HandleMessage(UINT _In_ uMsg,
	WPARAM _In_ wParam, LPARAM _In_ lParam)
{
	switch (uMsg)
	{

	case WM_COMMAND: {
		UNREFERENCED_PARAMETER(lParam);
		UINT nID = LOWORD(wParam);

		switch (nID) {
		case ID_PLAY_RANDOM: {
			m_refAE.PlayAudioSample(sample::nSample[rand() % sample::sCnt]);
		} break;
		case ID_PAUSE: {
			m_ap->PauseAudio();
		} break;

		case IDC_CURRENT_AUDIO_POSITION: {
			/*LONG lPos = SendMessage(
				m_conTrackBar.hWndTrack, TBM_GETPOS, 0x0, 0x0
			);*/

			LONG lPos = (LONG)roundf(m_ap->CurrentPositonAudio() * 100.f);

			Logger::ShowMessage
				(std::to_wstring(lPos).data(), L"[Range]");
		} break;

		default:
			break;
		}
	} break;

	case WM_HSCROLL: {
		HWND hWndTrack = (HWND)lParam;
		if (m_conTrackBar.hWndTrack != hWndTrack) { break; }

		if (LOWORD(wParam) == SB_ENDSCROLL) {
			m_ap->PositonAudio(m_conTrackBar.lPos / 100.f);
		}
		m_conTrackBar.lPos = HIWORD(wParam);
	} break;
	case WM_VSCROLL: {
		/*Code...*/
	} break;

	case WM_PAINT: {
		UNREFERENCED_PARAMETER(lParam), UNREFERENCED_PARAMETER(wParam);
		PAINTSTRUCT psPaint;
		HDC hdc = BeginPaint(m_hWnd, &psPaint);
			FillRect(hdc, &psPaint.rcPaint,
				reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
		(void)EndPaint(m_hWnd, &psPaint);
	} break;

	

	case WM_CLOSE: {
		UNREFERENCED_PARAMETER(lParam), UNREFERENCED_PARAMETER(wParam);
		if(!DestroyDlg(EXIT_SUCCESS))
		{ Logger::ShowLastError
			(L"DestroyDlg(nExitCode)");
		}
		PostQuitMessage(EXIT_SUCCESS);
	} break;



	default:
		break;
	}
	return(0x0);
}
