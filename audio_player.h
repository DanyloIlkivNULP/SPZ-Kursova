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

	typedef AudioEngine::AudioSample*
		pAudioSample;
	typedef AudioEngine::PlayingAudio*
		pPlayingAudio;
private:
	class AudioPlayerHandler :
		public AudioEngine::PlayingAudio
	{
		friend class AudioPlayer;

		typedef AudioPlayer*
			pAudioPlayer;
	public:
		AudioPlayerHandler(AUDIOID nAudioSampleID,
			pAudioPlayer pAP
		);
		~AudioPlayerHandler(void);

		float ProcessAudioSample(int nChannel,
			float fGlobalTime, float fTimeStep, float fMixerSample,
			const std::shared_ptr<AudioEngine::AudioSample>& pS
		);
	protected:
		pAudioPlayer m_pAP = nullptr;
	};

public:
	AudioPlayer
		(pAudioEngine pAE);
	~AudioPlayer(void);

	AudioPlayer(const AudioPlayer& ae) = delete;
	AudioPlayer& operator=(const AudioPlayer& ae) = delete;

	struct AE_DATA {
	DWORD dwSampleRate = 0x0;
	WORD wBitsPerSample = 0x0,
		wChannels = 0x0;

	DWORD dwBlockCount = 0x0,
		dwBlockSamples = 0x0;
	};

	typedef std::pair<std::shared_ptr
		<AudioEngine::AudioSample>, std::shared_ptr<AudioEngine::PlayingAudio>
	> AUDIO_DATA;

protected:
	pAudioEngine m_pAE = nullptr;

	AUDIO_DATA CreateAudio
		(AUDIOID nAudioSampleID);

	virtual float AudioHandler(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
			const pAudioSample pS, const pPlayingAudio pH
	) = 0x0;

	void AudioEngineData
		(AE_DATA& ae_data) const;
};

#endif // _AUDIO_PLAYER_H_
