#include "maindlg.h"
#include "resource.h"

#include "control_button.h"
#include "control_combobox.h"

#include "control_static_text.h"
#include "control_slider.h"

#include "audio_engine.h"
#include "main_audio_player.h"

#include "logger.h"

#define _TIMER_MAIN_ID_ (INT)0x0
#define _TIMER_MAIN_ELAPSE_ (INT)128

MainDlg::MainDlg(LPWSTR dlgResName, const wchar_t* wcWavFile, AudioEngine& refAE) :
	m_wcWavFile((wchar_t*)wcWavFile), m_refAE(refAE), BaseDlgBox(dlgResName)
{
	m_ap = std::make_unique
		<MainAudioPlayer>(&m_refAE);
}
MainDlg::~MainDlg(void) { /*Code...*/ }

bool MainDlg::OnUserCreate(void) {
	m_pPlay = std::make_unique
		<Button>(m_hWnd, ID_PLAY, L"[...]"
	);

	m_pPlayList = std::make_unique
		<Combobox>(m_hWnd, IDC_PLAYLIST);

	m_conStaticText.pFileName = std::make_unique
		<StaticText>(m_hWnd, IDC_FILE_NAME, L"[...]"
	);
	m_conStaticText.pDuration = std::make_unique
		<StaticText>(m_hWnd, IDC_DURATION, L"00:00:00"
	);

	m_conSlider.pAudioTrack = std::make_unique
		<Slider>(m_hWnd, IDC_AUDIO_TRACK, 0x0, POINT{ 0x0, 500 });
	m_conSlider.pVolume = std::make_unique
		<Slider>(m_hWnd, IDC_VOLUME, 50, POINT{ 0x0, 100 });
	m_conSlider.pPitch = std::make_unique
		<Slider>(m_hWnd, IDC_PITCH, 0x2, POINT{ 0x1, 0x3 });

	AUDIOID nMusic = m_refAE.
		LoadAudioSample(m_wcWavFile);
	if (nMusic != -(0x1)) {
		(bool)NewAudioMusic(nMusic);
		m_pPlayList.get()->
			AddItemString(m_wcWavFile);
	}

	(void)SetTimer(m_hWnd,
		_TIMER_MAIN_ID_, _TIMER_MAIN_ELAPSE_, NULL
	);

	return(true);
}
bool MainDlg::OnUserDestroy(void) { return(true); }

bool MainDlg::NewAudioMusic(AUDIOID nMusicID) {
	bool bResult = false;

	m_ap.get()->LoadAudio(nMusicID);

	m_conStaticText.pFileName.get()->
		SetText(m_ap.get()->FileName());

	const wchar_t* wcState[] = { L"Play", L"Pause" };
	m_pPlay.get()->SetText
		(wcState[m_ap.get()->CurrentStateAudio()]);

	m_ap.get()->VolumeAudio(
		(float)(m_conSlider.pVolume.get()->GetRange() - m_conSlider.pVolume.get()->GetPos()) /
		m_conSlider.pVolume.get()->GetRange()
	);
	m_ap.get()->PitchAudio(
		((m_conSlider.pPitch.get()->GetRangeParam().y - m_conSlider.pPitch.get()->GetPos() +
			m_conSlider.pPitch.get()->GetRangeParam().x) * 0.5f)
	);

	return(bResult);
}

LRESULT CALLBACK MainDlg::HandleMessage(UINT _In_ uMsg,
	WPARAM _In_ wParam, LPARAM _In_ lParam)
{
	switch (uMsg)
	{

	case WM_COMMAND: {
		UNREFERENCED_PARAMETER(lParam);
		UINT lwID = LOWORD(wParam);

		switch (lwID) {
		case ID_FILE: {
			WCHAR wcFileName[_STRING_SIZE_] = { 0x0 };
			BOOL bResult = WavFileName(wcFileName);

			AUDIOID nMusic = m_refAE.
				LoadAudioSample(wcFileName);
			if (nMusic != -(0x1)) { (void)NewAudioMusic(nMusic);
				m_pPlayList.get()->AddItemString(wcFileName);
			}
		} break;
		case ID_PLAY: {
			if (m_ap.get()) {
				switch (m_ap.get()->CurrentStateAudio()) {
					case MainAudioPlayer::STATE_PLAY:
					{ m_pPlay.get()->SetText(L"Play"); } break;
					case MainAudioPlayer::STATE_STOP:
					{ m_pPlay.get()->SetText(L"Pause"); } break;
				}
				m_ap.get()->SwapStateAudio();
			}
		} break;



		default:
			break;
		}

		UINT hwID = HIWORD(wParam);

		switch (hwID) {
		case CBN_SELCHANGE: {
			HWND hWndCombobox = (HWND)lParam;
			if (m_ap.get() &&
				hWndCombobox == m_pPlayList.get()->GetHandle()
			)
			{
				DWORD dwID = m_pPlayList.get()->SelectedItemID() + 0x1;
				if (m_ap.get()->CurrentAudio() != dwID) {
					m_pPlay.get()->SetText(L"Play");
					(void)m_ap.get()->ChangeCurrentAudio
						((AUDIOID)dwID);

					m_ap.get()->ChangeStateAudio
						(MainAudioPlayer::STATE_STOP);
					m_ap.get()->PositonAudio(0.0);
				}
			}
		} break;

		default:
			break;
		}

	} break;

	case WM_HSCROLL: {
		HWND hWndTrack = (HWND)lParam;
		if (m_ap.get() &&
			hWndTrack == m_conSlider.pAudioTrack.get()->GetHandle()
		)
		{
			if (LOWORD(wParam) != SB_ENDSCROLL &&
				LOWORD(wParam) != SB_THUMBPOSITION)
			{
				DWORD dwPos = m_conSlider.pAudioTrack.get()->GetPos();
				m_ap.get()->PositonAudio(dwPos /
					(float)m_conSlider.pAudioTrack.get()->GetRange());

				m_bHold = 0x1;
				if (dwPos == m_conSlider.pAudioTrack.get()->GetRange())
				{ m_conSlider.pAudioTrack.get()->SetPos(0x0);
					m_ap.get()->ChangeStateAudio(MainAudioPlayer::STATE_STOP);
				}

				WCHAR wcDur[_STRING_SIZE_] = { 0x0 };

				double dCurrentPositionAudio = m_ap.get()->
					CurrentPositonAudio();
				float fNumOfSamples = (float)m_ap.get()->
					NumOfSamples();
				float fNumOfSamplesPerSec = (float)m_ap.get()->
					NumOfSamplesPerSec();

				AudioDuration(wcDur,
					dCurrentPositionAudio, fNumOfSamples, fNumOfSamplesPerSec);
				m_conStaticText.pDuration.get()->SetText(wcDur);
			}
			else { m_bHold = 0x0; }
			}
	} break;
	case WM_VSCROLL: {
		HWND hWndTrack = (HWND)lParam;
		if (m_ap.get() &&
			hWndTrack == m_conSlider.pVolume.get()->GetHandle()
		)
		{
			if (LOWORD(wParam) != SB_ENDSCROLL) {
				DWORD dwPos = m_conSlider.pVolume.get()->GetPos();
				m_ap.get()->VolumeAudio((float)(m_conSlider.pVolume.get()->GetRange() - dwPos) /
					m_conSlider.pVolume.get()->GetRange());
				

				m_bHold = 0x1;
			}
			else { m_bHold = 0x0; }
		}
		if (m_ap.get() &&
			hWndTrack == m_conSlider.pPitch.get()->GetHandle()
		)
		{
			if (LOWORD(wParam) != SB_ENDSCROLL) {
				DWORD dwPos = m_conSlider.pPitch.get()->GetPos();
				m_ap.get()->PitchAudio(
				((m_conSlider.pPitch.get()->GetRangeParam().y - dwPos +
					m_conSlider.pPitch.get()->GetRangeParam().x) * 0.5f)
				);

				m_bHold = 0x1;
			}
			else { m_bHold = 0x0; }
		}
	} break;

	case WM_TIMER: {
		INT nID = (INT)wParam;
		switch (nID)
		{

		case _TIMER_MAIN_ID_: {
			if (m_ap.get()) {
				WCHAR wcDur[_STRING_SIZE_] = { 0x0 };

				double dCurrentPositionAudio = m_ap.get()->
					CurrentPositonAudio();
				float fNumOfSamples = (float)m_ap.get()->
					NumOfSamples();
				float fNumOfSamplesPerSec = (float)m_ap.get()->
					NumOfSamplesPerSec();
				
				AudioDuration(wcDur,
					dCurrentPositionAudio, fNumOfSamples, fNumOfSamplesPerSec);
				m_conStaticText.pDuration.get()->SetText(wcDur);
			}

			if (m_bHold) { break; }
			if (m_ap.get()) {
				m_conSlider.pAudioTrack.get()->SetPos
				((DWORD)(m_ap.get()->CurrentPositonAudio() *
					m_conSlider.pAudioTrack.get()->GetRange())
				);
			}
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

bool MainDlg::WavFileName
	(wchar_t wcFileName[_STRING_SIZE_])
{
	BOOL bResult = 0x0;

	OPENFILENAMEW hFile;
	ZeroMemory(&hFile,
		sizeof(hFile)
	);
	hFile.hwndOwner = m_hWnd;

	hFile.lpstrFile = wcFileName;
	hFile.nMaxFile = _STRING_SIZE_;

	hFile.lpstrFilter = L"*.wav\0";
	hFile.lpstrFileTitle = NULL;
	hFile.nMaxFileTitle = 0x0;

	hFile.lpstrInitialDir = NULL;

	hFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	hFile.lStructSize = sizeof(hFile);

	bResult = GetSaveFileName(&hFile);
	return(bResult);
}

void MainDlg::AudioDuration(wchar_t wcAudioDuration[_STRING_SIZE_],
	double dCurrentPositionAudio, float fNumOfSamples, float fNumOfSamplesPerSec)
{
	size_t sElapsedSec = (size_t)round(dCurrentPositionAudio *
		(fNumOfSamples / fNumOfSamplesPerSec));
	size_t sElapsedMin = (size_t)
		(sElapsedSec / 60.f);
	size_t sElapsedHour = (size_t)
		(sElapsedMin / 60.f);

	ZeroMemory(wcAudioDuration,
		sizeof(wchar_t) * _STRING_SIZE_);
	swprintf_s((wchar_t*)wcAudioDuration, _STRING_SIZE_, L"%02i:%02i:%02i",
		sElapsedHour % 60, sElapsedMin % 60, sElapsedSec % 60);
}
