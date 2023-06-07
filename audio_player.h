#pragma once
#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include "audio_engine.h"

#include "playing_audio.h"

class AudioEngine::AudioSample;

class AudioEngine::PlayingAudio;

class AudioPlayer {

protected:
	typedef AudioEngine*
		pAudioEngine;
private:
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

	typedef std::pair<std::shared_ptr
		<AudioEngine::AudioSample>, std::shared_ptr<AudioEngine::PlayingAudio>
	> AUDIO_DATA;

protected:
	pAudioEngine m_pAE = nullptr;

	AUDIO_DATA CreateAudio
		(AUDIOID nAudioSampleID);

	virtual float AudioHandler(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
	) = 0x0;
};

#endif // _AUDIO_PLAYER_H_
