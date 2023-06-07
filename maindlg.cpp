#include "maindlg.h"
#include "resource.h"

#include "control_static_text.h"
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
MainDlg::~MainDlg(void) { /*Code...*/ }

bool MainDlg::OnUserCreate(void) {
	m_conStaticText.pFileName = std::make_unique
		<StaticText>(m_hWnd, IDC_FILE_NAME,
			m_ap.get()->FileName()
	);
	m_conStaticText.pDuration = std::make_unique
		<StaticText>(m_hWnd, IDC_DURATION, L"00:00:00"
	);

	m_conAudioTrack.pSlider = std::make_unique
		<Slider>(m_hWnd, IDC_AUDIO_TRACK, 0x0, POINT{ 0x0, 100 });
	m_conVolume.pSlider = std::make_unique
		<Slider>(m_hWnd, IDC_VOLUME, 150, POINT{ 0x0, 300 });

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
		if (hWndTrack == m_conAudioTrack.pSlider->GetHandle())
			if (LOWORD(wParam) != SB_ENDSCROLL) {
				DWORD dwPos = m_conAudioTrack.pSlider->GetPos();
				{ m_ap->PositonAudio(dwPos / 100.f); }

				m_conAudioTrack.bHold = 0x1;
				if (dwPos == 100)
				{ m_conAudioTrack.pSlider->SetPos(0x0);
					m_ap.get()->PauseAudio(false);
				}

				WCHAR wcDur[256] = { 0x0 }; CalcDuration(wcDur);
				m_conStaticText.pDuration.get()->SetText(wcDur);
			}
			else { m_conAudioTrack.bHold = 0x0; }
	} break;
	case WM_VSCROLL: {
		HWND hWndTrack = (HWND)lParam;
		if (hWndTrack == m_conVolume.pSlider->GetHandle())
			if (LOWORD(wParam) != SB_ENDSCROLL) {
				DWORD dwPos = m_conVolume.pSlider->GetPos();
				{ m_ap->VolumeAudio((300 - dwPos) / 300.f); }

				m_conVolume.bHold = 0x1;
			}
			else { m_conVolume.bHold = 0x0; }
	} break;

	case WM_TIMER: {
		INT nID = (INT)wParam;
		switch (nID)
		{

		case _TIMER_MAIN_ID_: {
			if (m_conAudioTrack.bHold) { break; }
			m_conAudioTrack.pSlider->SetPos
				((DWORD)(m_ap.get()->CurrentPositonAudio() * 100.f));

			WCHAR wcDur[256] = { 0x0 }; CalcDuration(wcDur);
			m_conStaticText.pDuration.get()->SetText(wcDur);
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

void MainDlg::CalcDuration
	(wchar_t wcDuration[_DURATION_SIZE_])
{
	size_t sElapsedSec = (size_t)roundf(m_ap.get()->CurrentPositonAudio() *
		(m_ap.get()->NumOfSamples() / m_ap.get()->NumOfSamplesPerSec()));
	size_t sElapsedMin = (size_t)
		(sElapsedSec / 60.f);
	size_t sElapsedHour = (size_t)
		(sElapsedMin / 60.f);

	ZeroMemory(wcDuration,
		sizeof(wchar_t) * _DURATION_SIZE_);
	swprintf_s((wchar_t*)wcDuration, _DURATION_SIZE_, L"%02i:%02i:%02i",
		sElapsedHour % 60, sElapsedMin % 60, sElapsedSec % 60);
}
