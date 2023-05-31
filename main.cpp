#include "framework.h"

#include "audio_engine.h"
#include "console_api.h"
#include "logger.h"

#include <conio.h>
using namespace std;

// Global synthesizer variables
atomic<float> fFrequencyOutput = 0.f;			// dominant output frequency of instrument, i.e. the note
float fOctaveBaseFrequency = 3.f * 110.f;		// frequency of octave represented by keyboard
float f12thRootOf2 = powf(2.f, 1.f / 12.f);		// assuming western 12 notes per ocatve

// Function used by olcNoiseMaker to generate sound waves
// Returns amplitude (-1.0 to +1.0) as a function of time
float MakeNoise(int nChannel,
	float fGlobalTime, float fTimeStep)
{
	float fOutput = sinf(fFrequencyOutput * 2.f * 3.14159f * fGlobalTime) + sinf((fFrequencyOutput + 5.f) * 2.f * 3.14159f * fGlobalTime);
	return(fOutput * 0.5f); // Master Volume
}

int wmain(void) {
	Logger::LoadLogLevel
		(Logger::LogLevel::LOG_LVL_DEBUG);
	Logger::ClearLog();

	ConstructConsole(90, 30, L"[AudioEngine]-TEST-0",
		10, 16, L"Consolas", L"C");

	AudioEngine audio;
	audio.CreateAudio(&MakeNoise);

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
					L"Failed to Load : " + (wstring)cSample[i]);
			{ return(ERROR_INVALID_DATA); }
		}
	}

	wint_t wSygn = L'\0';
	int nCurrentKey = -0x1;
	bool bKeyPressed = false;

	while (wSygn != L'\r') {
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
		bKeyPressed = false;
		for (int k = 0; k < 16; k++)
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000) {
				if (nCurrentKey != k && !bKeyPressed) {
					fFrequencyOutput = fOctaveBaseFrequency * powf(f12thRootOf2, (float)k);
					nCurrentKey = k;
				}

				bKeyPressed = true;
			}
		if (!bKeyPressed) {
			if (nCurrentKey != -1)
			{ nCurrentKey = -1; }
			fFrequencyOutput = 0.f;
		}
	}

	audio.DestroyAudio();

	return(EXIT_SUCCESS);
}
