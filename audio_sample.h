#pragma once
#ifndef _AUDIO_SAMPLE_H_
#define _AUDIO_SAMPLE_H_

#include "audio_engine.h"

class AudioEngine::AudioSample
{
	friend class AudioEngine;
	bool m_bValid = false;
public:
	AudioSample(void); ~AudioSample(void);

	AudioSample(std::wstring sWavFile);

protected:
	WAVEFORMATEX wavHeader = { 0x0 };

	float* m_fSample = nullptr;
	long m_nSamples = 0x0;
	int m_nChannels = 0x0;

	virtual bool LoadAudioSample
		(std::wstring sWavFile);
};

#endif //_AUDIO_SAMPLE_H_
