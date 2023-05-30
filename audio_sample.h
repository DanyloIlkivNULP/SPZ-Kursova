#pragma once
#ifndef _AUDIO_SAMPLE_H_
#define _AUDIO_SAMPLE_H_

#include "audio_engine.h"

class AudioEngine::AudioSample {

public:
	AudioSample(void); ~AudioSample(void);

	AudioSample(std::wstring sWavFile);

	WAVEFORMATEX wavHeader = { 0x0 };

	float* fSample = nullptr;
	long nSamples = 0x0;
	int nChannels = 0x0;

	bool bSampleValid = false;
};

#endif //_AUDIO_SAMPLE_H_
