#pragma once
#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include "framework.h"

class AudioPlayer;

typedef signed int AUDIOID;
typedef std::function
	<float(int, float, float)> AUDIO_HANDLER;

class AudioEngine {
	friend class AudioPlayer;
public:
	AudioEngine(
		AUDIO_HANDLER fSoundSample = nullptr,
		AUDIO_HANDLER fSoundFilter = nullptr
	); ~AudioEngine(void);

	AudioEngine(const AudioEngine& ae) = delete;
	AudioEngine& operator=(const AudioEngine& ae) = delete;

	virtual AUDIOID LoadAudioSample(std::wstring sWavFile);
	virtual void PlayAudioSample(AUDIOID ID);



	virtual bool CreateAudio(
		unsigned int nSampleRate = 44100, unsigned int nChannels = 0x1,
		unsigned int nBlocks = 0x8, unsigned int nBlockSamples = 512
	);
	virtual bool DestroyAudio(void);



	float GetGlobalTime(void) const;

private:
	void waveOutProc(HWAVEOUT hWaveOut,
		UINT uMsg, DWORD dwParam1, DWORD dwParam2);

	static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut,
		UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2
	);

	void AudioThread(void);

protected:
	unsigned int m_nSampleRate = 0x0;
	unsigned int m_nChannels = 0x0;
	unsigned int m_nBlockCount = 0x0;
	unsigned int m_nBlockSamples = 0x0;
	unsigned int m_nBlockCurrent = 0x0;

	short* m_pBlockMemory = nullptr;
	WAVEHDR* m_pWaveHeaders = nullptr;
	HWAVEOUT m_hwDevice = nullptr;

	std::atomic<float> m_fGlobalTime = 0.f;

	std::thread m_AudioThread;
	std::atomic<bool>
		m_bAudioThreadActive = false;
	std::atomic<unsigned int> m_nBlockFree = 0x0;

	std::mutex m_muxProcessAudio;

	std::mutex m_muxAudioThreadDestroy;
	std::condition_variable m_cvAudioThreadDestroy;

	std::mutex m_muxBlockNotZero;
	std::condition_variable m_cvBlockNotZero;

	AUDIO_HANDLER
		m_fUserSoundSample = nullptr,
		m_fUserSoundFilter = nullptr;

	class AudioSample;

	// This vector holds all loaded sound samples in memory
	std::vector<std::shared_ptr<AudioSample>>
		vecAudioSamples;

	// This class represents a sound that is currently playing. It only
	// holds the sound ID and where this instance of it is up to for its
	// current playback
	class PlayingAudio;

	std::list<std::shared_ptr<PlayingAudio>>
		listActiveSamples;

	virtual float GetMixerOutput(int nChannel,
		float fGlobalTime, float fTimeStep);

	template<class AudioSampleType, typename ...ArgumentTypes>
	AUDIOID LoadAudioSample(std::wstring sWavFile, ArgumentTypes&& ...args) {
		std::shared_ptr<AudioSampleType> a = std::make_shared
			<AudioSampleType>(sWavFile, std::forward<ArgumentTypes>(args)...);

		if (a->m_bValid) {
			vecAudioSamples.push_back(std::move((std::shared_ptr<AudioSample>)a));
			return(vecAudioSamples.size());
		}
		else
		{ return(-0x1); }
	}
	template<class CreatingPlayingAudioType, typename ...ArgumentTypes>
	void CreatePlayingAudio(AUDIOID ID, ArgumentTypes&& ...args) {
		std::shared_ptr<CreatingPlayingAudioType> s = std::make_shared
			<CreatingPlayingAudioType>(ID, std::forward<ArgumentTypes>(args)...);
		listActiveSamples.push_back(std::move(s));
	}
};

#endif //_AUDIO_ENGINE_H_
