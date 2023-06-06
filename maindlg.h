#ifndef _MAINDLG_
#define _MAINDLG_

#include "framework.h"
#include "basedlgbox.h"

class AudioEngine;

class AudioPlayer;

class MainDlg : 
	public BaseDlgBox
{
	AudioEngine& m_refAE;

	std::unique_ptr
		<AudioPlayer> m_ap = nullptr;
public:
	MainDlg(LPWSTR dlgResName, AudioEngine& refAE);
	virtual ~MainDlg(void);
private:
	LRESULT CALLBACK HandleMessage(UINT _In_ uMsg,
		WPARAM _In_ wParam, LPARAM _In_ lParam);

	bool OnUserCreate(void); bool OnUserDestroy(void);

	struct {
		HWND hWndTrack = NULL;
		LONG lPos = 0x0;
	} m_conTrackBar;
};

#endif //_MAINDLG_
