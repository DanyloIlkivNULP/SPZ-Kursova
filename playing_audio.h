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

	std::atomic<double>
		m_dSamplePosition = 0.0;
	std::atomic<bool>
		m_bFinish = false;

	AUDIOID AudioSampleID(void) const;

protected:
	virtual float ProcessAudioSample(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
	);
};

#endif //_PlAYING_AUDIO_SAMPLE_H_
