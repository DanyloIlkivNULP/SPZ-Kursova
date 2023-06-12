#include "maindlg.h"
#include "resource.h"

#include "control_button.h"
#include "control_combobox.h"

#include "control_static_text.h"
#include "control_slider.h"

#include "audio_engine.h"

#include "logger.h"

#define _TIMER_MAIN_ID_ (INT)0x0
#define _TIMER_MAIN_ELAPSE_ (INT)128

#define _FILE_FORMAT_ L"*.wav\n"
#define _NULL_STRING_ L"[NULL_STATE]"

#define _NULL_DURATION_ L"00:00:00"
#define _DURATION_ L"%02i:%02i:%02i"

MainDlg::MainDlg(LPWSTR dlgResName, const wchar_t* wcWavFile, AudioEngine& refAE) :
	m_wcWavFile((wchar_t*)wcWavFile), m_refAE(refAE), m_audioPlayer(&m_refAE), BaseDlgBox(dlgResName)
{
	/*Code...*/
}
MainDlg::~MainDlg(void) { /*Code...*/ }

bool MainDlg::OnUserCreate(void) {
	DragAcceptFiles
		(m_hWnd, TRUE);
	m_pPBState = std::make_unique
		<Button>(m_hWnd, ID_PB_STATE,
			L"[/]", 0x1
	);

	m_pPlay = std::make_unique
		<Button>(m_hWnd, ID_PLAY,
			_NULL_STRING_, 0x0
	);

	m_pPlayList = std::make_unique
		<Combobox>(m_hWnd, IDC_PLAYLIST, 0x0
	);

	m_conStaticText.pInfo = std::make_unique
		<StaticText>(m_hWnd, IDC_INFO,
			_NULL_STRING_, 0x0
	);
	m_conStaticText.pFileName = std::make_unique
		<StaticText>(m_hWnd, IDC_FILE_NAME,
			_NULL_STRING_, 0x0
	);
	m_conStaticText.pDuration = std::make_unique
		<StaticText>(m_hWnd, IDC_DURATION,
			_NULL_DURATION_, 0x0
	);

	m_conSlider.pAudioTrack = std::make_unique
		<Slider>(m_hWnd, IDC_AUDIO_TRACK, 0x0, POINT{ 0x0, 1000 });
	m_conSlider.pVolume = std::make_unique
		<Slider>(m_hWnd, IDC_VOLUME, 50, POINT{ 0x0, 100 });
	m_conSlider.pPitch = std::make_unique
		<Slider>(m_hWnd, IDC_PITCH, 0x2, POINT{ 0x1, 0x3 });

	(void)NewAudioMusic(m_wcWavFile);

	(void)SetTimer(m_hWnd,
		_TIMER_MAIN_ID_, _TIMER_MAIN_ELAPSE_, NULL
	);

	return(true);
}
bool MainDlg::OnUserDestroy(void) { return(true); }

bool MainDlg::NewAudioMusic(const wchar_t* wcWavFile) {
	if (wcWavFile == NULL) { return(false); }

	AUDIOID nMusicID = m_audioPlayer.
		LoadAudioSample(wcWavFile);

	if (nMusicID == -(0x1))
	{ Logger::ShowMessage(wcWavFile,
		L"Failed to Load File!"); return(false);
	}

	m_audioPlayer.ChangeStateAudio
		(MainAudioPlayer::STATE_STOP);
	(void)ChangeAudioMusic(nMusicID);

	WCHAR wcFileName[MAX_PATH];
	WCHAR wcExt[MAX_PATH];

	_wsplitpath_s(wcWavFile, NULL, 0x0, NULL, 0x0, wcFileName, MAX_PATH, wcExt, MAX_PATH);

	m_pPlayList.get()->
		AddItemString(wcFileName);
	m_pPlayList.get()->ChangeState(0x1);

	return(true);
}

bool MainDlg::ChangeAudioMusic(
	AUDIOID nMusicID
)
{
	bool bResult = false;

	if (nMusicID != -0x1)
		{ bResult = true; }
	(void)m_audioPlayer.
		ChangeCurrentAudioSample(nMusicID);

	m_audioPlayer.PositonAudio(NULL);

	m_conStaticText.pFileName.get()->
		ChangeState(nMusicID != -0x1);
	m_conStaticText.pFileName.get()->
		SetText(nMusicID != -0x1 ? m_audioPlayer.FileName() : _NULL_STRING_);

	m_pPlayList.get()->
		SelectItem(nMusicID != -0x1 ?
			nMusicID - 0x1 : -0x1
		);

	m_conStaticText.pInfo.get()->
		ChangeState(nMusicID != -0x1);
	std::wstring wsInfo = L"Info...";
	m_conStaticText.pInfo.get()->SetText(nMusicID != -0x1 ? wsInfo.data() : _NULL_STRING_);

	m_pPlay.get()->
		ChangeState(nMusicID != -0x1);
	WCHAR* wcState[0x2] = { (wchar_t*)L"Play", (wchar_t*)L"Pause" };
	m_pPlay.get()->SetText
		(nMusicID != -0x1 ?
			wcState[m_audioPlayer.CurrentStateAudio()] :
			_NULL_STRING_
		);

	m_conStaticText.pDuration.get()->
		ChangeState(nMusicID != -0x1);
	m_conStaticText.pDuration.get()->SetText(_NULL_DURATION_);

	m_audioPlayer.VolumeAudio(
		(float)(m_conSlider.pVolume.get()->GetRange() - m_conSlider.pVolume.get()->GetPos()) /
			m_conSlider.pVolume.get()->GetRange()
	);
	m_audioPlayer.PitchAudio(
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
			WCHAR wcFileName[MAX_PATH] = { 0x0 };
			BOOL bResult = WavFileName(wcFileName);

			if (bResult == TRUE)
			{ (void)NewAudioMusic
				(wcFileName);
			}
		} break;
		case ID_PLAY: {
			if (m_audioPlayer.CurrentAudioSample() != -0x1) {
				switch (m_audioPlayer.CurrentStateAudio()) {
					case MainAudioPlayer::STATE_PLAY:
					{ m_pPlay.get()->SetText(L"Play"); } break;
					case MainAudioPlayer::STATE_STOP:
					{ m_pPlay.get()->SetText(L"Pause"); } break;
				}
				m_audioPlayer.SwapStateAudio();
			}
		} break;
		case ID_PB_STATE: {
			m_iPB = (m_iPB + 0x1) % 0x3;
			WCHAR* wcState[0x3] = {
				(wchar_t*)L"[/]",
				(wchar_t*)L"[∞]",
				(wchar_t*)L"[→]"
			};
			m_pPBState.get()->SetText(wcState[m_iPB]);
		} break;
		case ID_X: {
			m_audioPlayer.ChangeStateAudio
				(MainAudioPlayer::STATE_STOP);
			(void)ChangeAudioMusic(-0x1);
		} break;

		case ID_HIDE_WINDOW: {
			BOOL bVisible = IsWindowVisible(m_hWnd);
			ShowWindow(m_hWnd, !bVisible ?
				SW_MAXIMIZE : SW_MINIMIZE
			);
		} break;

		default:
			break;
		}

		UINT hwID = HIWORD(wParam);

		switch (hwID) {
		case CBN_SELCHANGE: {
			HWND hWndCombobox = (HWND)lParam;
			if (hWndCombobox == m_pPlayList.get()->GetHandle()) {
				INT ID = m_pPlayList.get()->SelectedItemID() + 0x1;
				if (m_audioPlayer.CurrentAudioSample() != ID) {
					m_audioPlayer.ChangeStateAudio
						(MainAudioPlayer::STATE_STOP);
					(void)ChangeAudioMusic(ID);
				}
			}
		} break;

		default:
			break;
		}

	} break;

	case WM_HSCROLL: {
		HWND hWndTrack = (HWND)lParam;
		if (hWndTrack == m_conSlider.pAudioTrack.get()->GetHandle())
		{
			if (LOWORD(wParam) != SB_ENDSCROLL &&
				LOWORD(wParam) != SB_THUMBPOSITION)
			{
				DWORD dwPos = m_conSlider.pAudioTrack.get()->GetPos();
				m_audioPlayer.PositonAudio(dwPos /
					(float)m_conSlider.pAudioTrack.get()->GetRange());
				m_bHold = 0x1;

				WCHAR wcDur[MAX_PATH] = { 0x0 };

				double dCurrentPositionAudio = m_audioPlayer.
					CurrentPositonAudio();
				float fNumOfSamples = (float)m_audioPlayer.
					NumOfSamples();
				float fNumOfSamplesPerSec = (float)m_audioPlayer.
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
		if (hWndTrack == m_conSlider.pVolume.get()->GetHandle())
		{
			if (LOWORD(wParam) != SB_ENDSCROLL) {
				DWORD dwPos = m_conSlider.pVolume.get()->GetPos();
				m_audioPlayer.VolumeAudio((float)(m_conSlider.pVolume.get()->GetRange() - dwPos) /
					m_conSlider.pVolume.get()->GetRange());
			}
		}
		if (hWndTrack == m_conSlider.pPitch.get()->GetHandle())
		{
			if (LOWORD(wParam) != SB_ENDSCROLL) {
				DWORD dwPos = m_conSlider.pPitch.get()->GetPos();
				m_audioPlayer.PitchAudio(
				((m_conSlider.pPitch.get()->GetRangeParam().y - dwPos +
					m_conSlider.pPitch.get()->GetRangeParam().x) * 0.5f)
				);
			}
		}
	} break;

	case WM_TIMER: {
		INT nID = (INT)wParam;
		switch (nID)
		{

		case _TIMER_MAIN_ID_: {
			if (m_bHold) { break; }
			if (m_audioPlayer.CurrentAudioSample() != -0x1) {
				if (m_conSlider.pAudioTrack.get()->GetPos() ==
					m_conSlider.pAudioTrack.get()->GetRange()
				)
				{
					m_audioPlayer.PositonAudio(0.0);
					switch (m_iPB) {
					case PB_STATE_STOP: {
						m_audioPlayer.ChangeStateAudio
							(MainAudioPlayer::STATE_STOP);
						m_pPlay.get()->SetText(L"Play");
					} break;
					case PB_STATE_REPEAT: {
						m_audioPlayer.ChangeStateAudio
							(MainAudioPlayer::STATE_PLAY);
					} break;
					case PB_STATE_CONTINUE: {
						INT iCount = m_pPlayList.
							get()->CountOfItems();
						m_audioPlayer.ChangeStateAudio
							(MainAudioPlayer::STATE_PLAY);
						if (iCount > 0x1) {
							INT ID = (m_audioPlayer.
								CurrentAudioSample() % iCount
							);
							(void)ChangeAudioMusic(ID + 0x1);
						}
					} break;

					default:
						break;
					}
				}
				m_conSlider.pAudioTrack.get()->SetPos
				((DWORD)(m_audioPlayer.CurrentPositonAudio() *
					m_conSlider.pAudioTrack.get()->GetRange())
				);

				WCHAR wcDur[MAX_PATH] = { 0x0 };

				double dCurrentPositionAudio = m_audioPlayer.
					CurrentPositonAudio();
				float fNumOfSamples = (float)m_audioPlayer.
					NumOfSamples();
				float fNumOfSamplesPerSec = (float)m_audioPlayer.
					NumOfSamplesPerSec();

				AudioDuration(wcDur,
					dCurrentPositionAudio, fNumOfSamples, fNumOfSamplesPerSec);
				m_conStaticText.pDuration.get()->SetText(wcDur);
			}
		}

		default:
			break;
		}
	}

	case WM_DROPFILES: {
		HDROP hDrop = (HDROP)wParam;
		if (hDrop != NULL) {
			wchar_t wcFileName[MAX_PATH]{};

			size_t sCnt = DragQueryFile(hDrop,
				0xFFFFFFFF, NULL, 0x0
			);
			for (size_t i = 0x0; i < sCnt; i++) {
				DragQueryFile(hDrop,
					i, wcFileName, MAX_PATH
				);
				(void)NewAudioMusic(wcFileName);
				ZeroMemory(&wcFileName,
					sizeof(wcFileName)
				);
			}
			DragFinish(hDrop);
		}
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

bool MainDlg::WavFileName
	(wchar_t wcFileName[MAX_PATH])
{
	BOOL bResult = 0x0;

	OPENFILENAMEW hFile;
	ZeroMemory(&hFile,
		sizeof(hFile)
	);
	hFile.hwndOwner = m_hWnd;

	hFile.lpstrFile = wcFileName;
	hFile.nMaxFile = MAX_PATH;

	hFile.lpstrFilter = _FILE_FORMAT_;
	hFile.lpstrFileTitle = NULL;
	hFile.nMaxFileTitle = 0x0;

	hFile.lpstrInitialDir = NULL;

	hFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	hFile.lStructSize = sizeof(hFile);

	bResult = GetSaveFileName(&hFile);
	return(bResult);
}

void MainDlg::AudioDuration(wchar_t wcAudioDuration[MAX_PATH],
	double dCurrentPositionAudio, float fNumOfSamples, float fNumOfSamplesPerSec)
{
	size_t sElapsedSec = (size_t)round(dCurrentPositionAudio *
		(fNumOfSamples / fNumOfSamplesPerSec));
	size_t sElapsedMin = (size_t)
		(sElapsedSec / 60.f);
	size_t sElapsedHour = (size_t)
		(sElapsedMin / 60.f);

	ZeroMemory(wcAudioDuration,
		sizeof(wchar_t) * MAX_PATH);
	(void)swprintf_s((wchar_t*)wcAudioDuration, MAX_PATH, _DURATION_,
		sElapsedHour % 60, sElapsedMin % 60, sElapsedSec % 60);
}
