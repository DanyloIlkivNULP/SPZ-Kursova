#ifndef _MAINDLG_H_
#define _MAINDLG_H_

#include "framework.h"
#include "basedlgbox.h"

class AudioEngine;

class Button;
class StaticText;

class Combobox;
class Slider;

class MainAudioPlayer;

class MainDlg : 
	public BaseDlgBox
{
	AudioEngine& m_refAE;

	std::unique_ptr
		<MainAudioPlayer> m_ap = nullptr;

	wchar_t* m_wcWavFile = NULL;

	bool NewAudioMusic
		(const wchar_t* wcWavFile);
	bool ChangeAudioMusic
		(AUDIOID nMusicID);
public:
	MainDlg(LPWSTR dlgResName,
		const wchar_t* wcWavFile, AudioEngine& refAE);
	~MainDlg(void);
private:
	LRESULT CALLBACK HandleMessage(UINT _In_ uMsg,
		WPARAM _In_ wParam, LPARAM _In_ lParam);

	bool OnUserCreate(void); bool OnUserDestroy(void);

	std::unique_ptr
		<Button> m_pPlay = nullptr;

	std::unique_ptr
		<Combobox> m_pPlayList = nullptr;

	struct {
		std::unique_ptr<StaticText>
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
