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
		friend class AudioPlayer;

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

	AudioPlayer(const AudioPlayer& ae) = delete;
	AudioPlayer& operator=(const AudioPlayer& ae) = delete;

	void PauseAudio
		(bool bState);
	void PauseAudio(void);

	void PositonAudio
		(float fSamplePosition);
	float CurrentPositonAudio
		(void) const;

	DWORD NumOfSamples
		(void) const;
	DWORD NumOfSamplesPerSec
		(void) const;

protected:
	pAudioEngine m_pAE = nullptr;

	std::shared_ptr
		<AudioEngine::AudioSample> m_pAS = nullptr;
	std::shared_ptr
		<AudioEngine::PlayingAudio> m_pCPA = nullptr;

	bool m_bPause = false;

	virtual float AudioHandler(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
	);
};

#endif // _AUDIO_PLAYER_H_
