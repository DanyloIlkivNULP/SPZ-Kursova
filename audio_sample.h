#pragma once
#ifndef _AUDIO_SAMPLE_H_
#define _AUDIO_SAMPLE_H_

#include "framework.h"
#include "audio_engine.h"

class AudioPlayer;

class AudioEngine::AudioSample
{
	friend class AudioEngine;
	friend class AudioPlayer;

	bool m_bValid = false;
public:
	AudioSample(void); ~AudioSample(void);
	AudioSample(const wchar_t*
		wcWavFile);

	WAVEFORMATEX wavHeader = { 0x0 };

	float* m_fSample = nullptr;
	long m_nSamples = 0x0;
	int m_nChannels = 0x0;

	std::wstring m_wsWavFile;

protected:
	virtual bool LoadAudioSample
		(const wchar_t* wcWavFile);
};

#endif //_AUDIO_SAMPLE_H_
