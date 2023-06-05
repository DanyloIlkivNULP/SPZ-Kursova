#pragma once
#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include "audio_engine.h"

class AudioEngine::AudioSample;

class AudioEngine::PlayingAudio;

class AudioEngine::AudioPlayer {
	typedef AudioEngine* pAudioEngine;

	std::unique_ptr<AudioEngine> m_pAE = nullptr;
	struct {
		std::shared_ptr<AudioSample> m_pAS;
		std::shared_ptr<PlayingAudio> m_pCPA;
	} m_sData = { 0x0 };

public:
	AudioPlayer(AUDIOID nAudioSampleID,
		pAudioEngine pAE);
	~AudioPlayer(void);
};

#endif // _AUDIO_PLAYER_H_
