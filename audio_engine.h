#pragma once
#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include "framework.h"

class AudioEngine {
	typedef std::function
		<float(int, float, float)> audio_handler;
	
public:
	AudioEngine(
		audio_handler fSoundSample = nullptr,
		audio_handler fSoundFilter = nullptr
	); ~AudioEngine(void);

	virtual int LoadAudioSample(std::wstring sWavFile);

	virtual void PlaySample(int id);
	virtual void StopSample(int id);

	virtual bool CreateAudio(
		unsigned int nSampleRate = 44100, unsigned int nChannels = 0x1,
		unsigned int nBlocks = 0x8, unsigned int nBlockSamples = 512
	);
	virtual bool DestroyAudio(void);



	float GetGlobalTime(void);

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

	std::mutex m_muxAudioThreadDestroy;
	std::condition_variable m_cvAudioThreadDestroy;

	std::mutex m_muxBlockNotZero;
	std::condition_variable m_cvBlockNotZero;

	audio_handler
		m_fUserSoundSample = nullptr,
		m_fUserSoundFilter = nullptr;

	class AudioSample;

	// This vector holds all loaded sound samples in memory
	std::vector<std::unique_ptr<AudioSample>>
		vecAudioSamples;

	// This class represents a sound that is currently playing. It only
	// holds the sound ID and where this instance of it is up to for its
	// current playback
	class PlayingAudioSample;

	std::list<std::unique_ptr<PlayingAudioSample>>
		listActiveSamples;

	virtual float GetMixerOutput(int nChannel,
		float fGlobalTime, float fTimeStep);
};

#endif //_AUDIO_ENGINE_H_
