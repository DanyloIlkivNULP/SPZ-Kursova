#ifndef _MAINDLG_
#define _MAINDLG_

#include "framework.h"
#include "basedlgbox.h"

class AudioEngine;
class AudioPlayer;

class StaticText;
class Slider;

class MainDlg : 
	public BaseDlgBox
{
	AudioEngine& m_refAE;

	std::unique_ptr
		<AudioPlayer> m_ap = nullptr;

	std::wstring m_sWavFile;
public:
	MainDlg(LPWSTR dlgResName,
		std::wstring sWavFile, AudioEngine& refAE);
	~MainDlg(void);
private:
	LRESULT CALLBACK HandleMessage(UINT _In_ uMsg,
		WPARAM _In_ wParam, LPARAM _In_ lParam);

	bool OnUserCreate(void); bool OnUserDestroy(void);

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

#define _DURATION_SIZE_ (size_t)256
	void CalcDuration
		(wchar_t wcDuration[_DURATION_SIZE_]);
};

#endif //_MAINDLG_
