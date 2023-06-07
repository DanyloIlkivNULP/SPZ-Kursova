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

	const wchar_t*
		FileName(void) const;

	enum {
		STATE_PLAY = 0x0,
		STATE_STOP = 0x0
	};

	void ChangeStateAudio
		(bool bState);
	void SwapStateAudio(void);
	bool CurrentStateAudio
		(void) const;

	void VolumeAudio
		(float fVolume);
	float CurrentVolume(void) const;

	void PitchAudio
		(float fPitch);
	float CurrentPitch(void) const;

	void PositonAudio
		(double dSamplePosition);
	double CurrentPositonAudio
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

	std::atomic<bool> m_bState = 0x0;
	std::atomic<float> m_fVolume = 1.f;
	std::atomic<float> m_fPitch = 1.f;

	virtual float AudioHandler(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
	);

	virtual float
		Clip(float& f);
	virtual double
		Clip(double& d);
};

#endif // _AUDIO_PLAYER_H_
