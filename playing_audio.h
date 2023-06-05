#pragma once
#ifndef _PlAYING_AUDIO_SAMPLE_H_
#define _PlAYING_AUDIO_SAMPLE_H_

#include "audio_engine.h"

class AudioEngine::AudioSample;

class AudioEngine::PlayingAudio
{
	friend class AudioEngine;
	AUDIOID m_nAudioSampleID = 0x0;
public:
	PlayingAudio
		(AUDIOID nAudioSampleID = -(0x1));
	~PlayingAudio(void);

	int AudioSampleID(void) const;
	bool IsFinish(void) const;

protected:
	float m_fSamplePosition = 0.f;
	bool m_bFinish = false;

	virtual float ProcessAudioSample(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
	);
};

#endif //_PlAYING_AUDIO_SAMPLE_H_
