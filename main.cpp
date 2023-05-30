#include "framework.h"

#include "audio_engine.h"
#include "console_api.h"
#include "logger.h"

#include <conio.h>
using namespace std;

int wmain(void) {
	Logger::LoadLogLevel
		(Logger::LogLevel::LOG_LVL_DEBUG);
	Logger::ClearLog();

	ConstructConsole(90, 30, L"[AudioEngine]-TEST-0",
		10, 16, L"Consolas", L"C");

	AudioEngine audio;
	audio.CreateAudio();

	const wchar_t* cSample[0x3] = {
		L"test_0.wav",
		L"test_1.wav",
		L"test_2.wav"
	};
	int nSample[0x3] = { 0x0 };

	for (int i = 0x0; i < sizeof(cSample) / sizeof(wchar_t*); i++) {
		nSample[i] = audio.LoadAudioSample(cSample[i]);
		if (nSample[i] == -(0x1)) {
			Logger::LogAndShowMessage
				(Logger::LogLevel::LOG_LVL_ERROR,
					L"LoadAudioSample - Failed!");
			{ return(ERROR_INVALID_DATA); }
		}
	}

	wint_t wSygn = L'\0';
	while (wSygn != L'\r')
		if (_kbhit()) {
			wSygn = _getwch();
			if (wSygn >= L'1' &&
				wSygn < L'1' + sizeof(cSample) / sizeof(wchar_t*)
			)
			{
				size_t nSampleNumber = wSygn - L'1';
				audio.PlaySample(nSample[nSampleNumber]);
				wcout <<
					L'\r' << L"                                                                " << L'\r';
				wcout <<
					L"Sample[" << nSampleNumber << L"] : " << cSample[nSampleNumber];
			}
		}

	audio.DestroyAudio();

	return(EXIT_SUCCESS);
}
