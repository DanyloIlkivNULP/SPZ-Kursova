#pragma once
#ifndef _PlAYING_AUDIO_SAMPLE_H_
#define _PlAYING_AUDIO_SAMPLE_H_

#include "audio_engine.h"

class AudioPlayer;

class AudioEngine::AudioSample;

class AudioEngine::PlayingAudio
{
	friend class AudioEngine;
	friend class AudioPlayer;

	AUDIOID m_nAudioSampleID = 0x0;
public:
	PlayingAudio
		(AUDIOID nAudioSampleID = -(0x1));
	~PlayingAudio(void);

	AUDIOID AudioSampleID(void) const;
	bool IsFinish(void) const;

protected:
	std::atomic<float> m_fSamplePosition = 0.f;
	std::atomic<bool> m_bFinish = false;

	virtual float ProcessAudioSample(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
	);
};

#endif //_PlAYING_AUDIO_SAMPLE_H_
