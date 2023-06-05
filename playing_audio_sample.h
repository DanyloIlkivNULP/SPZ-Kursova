#pragma once
#ifndef _PlAYING_AUDIO_SAMPLE_H_
#define _PlAYING_AUDIO_SAMPLE_H_

#include "audio_engine.h"

class AudioEngine::AudioSample;

class AudioEngine::PlayingAudioSample
{
	friend class AudioEngine;
	int m_nAudioSampleID = 0x0;
public:
	PlayingAudioSample
		(int nAudioSampleID = -(0x1));
	~PlayingAudioSample(void);

	int AudioSampleID(void) const;
	bool IsFinish(void) const;

protected:
	float m_fSamplePosition = 0.f;
	bool m_bFinish = false;

	virtual float ProcessAudioSample(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
		const AudioEngine::AudioSample* const pS
	);
};

#endif //_PlAYING_AUDIO_SAMPLE_H_
