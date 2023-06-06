#pragma once
#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include "audio_engine.h"

#include "playing_audio.h"

class AudioEngine::AudioSample;

class AudioEngine::PlayingAudio;

class AudioPlayer {
	typedef AudioEngine*
		pAudioEngine;

	class ActivePlayingAudio :
		public AudioEngine::PlayingAudio
	{
		typedef AudioPlayer*
			pAudioPlayer;
	public:
		ActivePlayingAudio(AUDIOID nAudioSampleID,
			pAudioPlayer pAP
		);
		~ActivePlayingAudio(void);

		float ProcessAudioSample(int nChannel,
			float fGlobalTime, float fTimeStep, float fMixerSample,
			const std::shared_ptr<AudioEngine::AudioSample>& pS
		);
	protected:
		pAudioPlayer m_pAP = nullptr;
	};

public:
	AudioPlayer(pAudioEngine pAE,
		AUDIOID nAudioSampleID);
	~AudioPlayer(void);

	void PauseAudio(void);

protected:
	pAudioEngine m_pAE = nullptr;
	std::shared_ptr
		<AudioEngine::PlayingAudio> m_pCPA = nullptr;

	bool m_bPause = false;
};

#endif // _AUDIO_PLAYER_H_
