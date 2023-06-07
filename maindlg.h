#ifndef _MAINDLG_
#define _MAINDLG_

#include "framework.h"
#include "basedlgbox.h"

class AudioEngine;
class AudioPlayer;

class Button;
class StaticText;
class Slider;

#define _STRING_SIZE_ (size_t)256

class MainDlg : 
	public BaseDlgBox
{
	AudioEngine& m_refAE;

	std::unique_ptr
		<AudioPlayer> m_ap = nullptr;

	wchar_t* m_wcWavFile = NULL;

	bool NewAudioPlayer
		(AUDIOID nMusicID);

	bool WavFileName
		(wchar_t wcFileName[_STRING_SIZE_]);
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

	struct {
		std::unique_ptr<StaticText>
			pFileName = nullptr,
			pDuration = nullptr;
	} m_conStaticText;

	struct {
		std::unique_ptr
			<Slider> pSlider = nullptr;
		bool bHold = 0x0;
	} m_conAudioTrack, m_conVolume;

	void AudioDuration
		(wchar_t wcAudioDuration[_STRING_SIZE_]);
};

#endif //_MAINDLG_
