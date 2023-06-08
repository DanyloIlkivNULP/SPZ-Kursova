#pragma once
#ifndef _MAIN_AUDIO_PLAYER_H_
#define _MAIN_AUDIO_PLAYER_H_

#include "audio_player.h"

class MainAudioPlayer :
	public AudioPlayer
{

public:
	MainAudioPlayer
		(pAudioEngine pAE);
	~MainAudioPlayer(void);

	MainAudioPlayer(const MainAudioPlayer& ae) = delete;
	MainAudioPlayer& operator=(const MainAudioPlayer& ae) = delete;

	const wchar_t*
		FileName(void) const;

	enum {
		STATE_STOP = 0x0,
		STATE_PLAY = 0x1
	};

	AUDIOID LoadAudio(AUDIOID ID);
	bool ChangeCurrentAudio
		(AUDIOID ID);
	AUDIOID CurrentAudio(void) const;

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

private:
	mutable std::vector
		<AUDIO_DATA> m_vecAudio;
	mutable std::atomic<AUDIOID> m_nCurrentAudio = -0x1;

	std::atomic<bool> m_bState = 0x0;
	std::atomic<float>
		m_fVolume = 1.f,
		m_fPitch = 1.f;

	mutable std::recursive_mutex m_muxData;

	virtual float AudioHandler(int nChannel,
		float fGlobalTime, float fTimeStep, float fMixerSample,
			const pAudioSample pS, const pPlayingAudio pA
	);

	AUDIO_DATA&
		AudioData(void) const;
	std::shared_ptr<AudioEngine::AudioSample>&
		AudioSample(void) const;
	std::shared_ptr<AudioEngine::PlayingAudio>&
		PlayingAudio(void) const;

	float Clip
		(float& f);
	double Clip
		(double& d);
};

#endif // _MAIN_AUDIO_PLAYER_H_
