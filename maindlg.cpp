#include "maindlg.h"
#include "resource.h"

#include "control_slider.h"

#include "audio_engine.h"
#include "audio_player.h"

#include "logger.h"

#define _TIMER_MAIN_ID_ (INT)0x0
#define _TIMER_MAIN_ELAPSE_ (INT)128

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

MainDlg::MainDlg(LPWSTR dlgResName, std::wstring sWavFile, AudioEngine& refAE) :
	m_sWavFile(sWavFile), m_refAE(refAE), BaseDlgBox(dlgResName)
{
	for (int i = 0x0; i < sizeof(sample::cSample) / sizeof(wchar_t*); i++) {
		sample::nSample[i] = m_refAE.LoadAudioSample(sample::cSample[i]);
		if (sample::nSample[i] == -(0x1))
		{ m_refAE.DestroyAudio(); }
	}

	AUDIOID nMusic = m_refAE.
		LoadAudioSample(m_sWavFile);
	if (nMusic == -(0x1))
	{ m_refAE.DestroyAudio(); }

	m_ap = std::make_unique
		<AudioPlayer>(&m_refAE, nMusic);
}
MainDlg::~MainDlg(void)
{ /*Code...*/ }

bool MainDlg::OnUserCreate(void) {
	m_conSlider.pSlider = new
		Slider(m_hWnd, IDC_SLIDER, { 0x0, 100 });

	(void)SetTimer(m_hWnd,
		_TIMER_MAIN_ID_, _TIMER_MAIN_ELAPSE_, NULL
	);

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

		default:
			break;
		}
	} break;

	case WM_HSCROLL: {
		HWND hWndTrack = (HWND)lParam;
		if (m_conSlider.pSlider->GetHandle() != hWndTrack) { break; }

		if (LOWORD(wParam) != SB_ENDSCROLL) { 
			DWORD dwPos = m_conSlider.pSlider->GetPos();
			{ m_ap->PositonAudio(dwPos / 100.f); }

			m_conSlider.bHold = 0x1;
			if (dwPos == 100)
			{ m_conSlider.pSlider->SetPos(0x0);
				m_ap.get()->PauseAudio(false);
			}
		}
		else { m_conSlider.bHold = 0x0; }
	} break;
	case WM_VSCROLL: {
		/*Code...*/
	} break;

	case WM_TIMER: {
		INT nID = (INT)wParam;
		switch (nID)
		{

		case _TIMER_MAIN_ID_: {
			HWND hStaticText = GetDlgItem
				(m_hWnd, IDC_DURATION);

			size_t sElapsedSec = (size_t)roundf(m_ap.get()->CurrentPositonAudio() *
				(m_ap.get()->NumOfSamples() / m_ap.get()->NumOfSamplesPerSec()));

			WCHAR wcDur[256] = { 0x0 };
			swprintf_s(wcDur, L"%02d:%02d:%02d",
				sElapsedSec / (60 * 60), sElapsedSec / 60, sElapsedSec % 60);
			SetWindowText(hStaticText, wcDur);

			if (m_conSlider.bHold) { break; }
			m_conSlider.pSlider->SetPos
				((DWORD)(m_ap.get()->CurrentPositonAudio() * 100.f));
		}

		default:
			break;
		}
	}

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
