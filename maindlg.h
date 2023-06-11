#ifndef _MAINDLG_H_
#define _MAINDLG_H_

#include "framework.h"
#include "basedlgbox.h"

class AudioEngine;

class Button;
class StaticText;

class Combobox;
class Slider;

#include "main_audio_player.h"

class MainDlg : 
	public BaseDlgBox
{
	AudioEngine& m_refAE;

	MainAudioPlayer m_audioPlayer;

	wchar_t* m_wcWavFile = NULL;

	bool NewAudioMusic
		(const wchar_t* wcWavFile);
	bool ChangeAudioMusic(
		AUDIOID nMusicID
	);
public:
	MainDlg(LPWSTR dlgResName,
		const wchar_t* wcWavFile, AudioEngine& refAE);
	~MainDlg(void);
private:
	LRESULT CALLBACK HandleMessage(UINT _In_ uMsg,
		WPARAM _In_ wParam, LPARAM _In_ lParam);

	bool OnUserCreate(void); bool OnUserDestroy(void);

	enum {
		PB_STATE_STOP = 0x0,
		PB_STATE_REPEAT = 0x1,
		PB_STATE_CONTINUE = 0x2
	};
	INT m_iPB = 0x0;

	std::unique_ptr
		<Button> m_pPlay = nullptr;
	std::unique_ptr
		<Button> m_pPBState = nullptr;

	std::unique_ptr
		<Combobox> m_pPlayList = nullptr;

	struct {
		std::unique_ptr<StaticText>
			pInfo = nullptr,
			pFileName = nullptr,
			pDuration = nullptr;
	} m_conStaticText;

	struct {
		std::unique_ptr
			<Slider> pAudioTrack = nullptr;
		std::unique_ptr
			<Slider> pVolume = nullptr;
		std::unique_ptr
			<Slider> pPitch = nullptr;
	} m_conSlider;

	bool m_bHold = 0x0;

	bool WavFileName
		(wchar_t wcFileName[MAX_PATH]);

	void AudioDuration(wchar_t wcAudioDuration[MAX_PATH],
		double dCurrentPositionAudio, float fNumOfSamples, float fNumOfSamplesPerSec);
};

#endif //_MAINDLG_H_
