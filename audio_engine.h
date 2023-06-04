#pragma once
#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include "framework.h"

class AudioEngine
{
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

	typedef std::function
		<float(int, float, float)> audio_handler;

	audio_handler
		m_fUserSoundSample = nullptr,
		m_fUserSoundFilter = nullptr;

	class AudioSample;

	// This vector holds all loaded sound samples in memory
	std::vector<AudioSample> vecAudioSamples;

	// This structure represents a sound that is currently playing. It only
	// holds the sound ID and where this instance of it is up to for its
	// current playback
	struct sCurrentlyPlayingSample {
		int nAudioSampleID = 0x0;
		long nSamplePosition = 0x0;
		bool bLoop = false;

		bool bFinished = false;
	};
	std::list<sCurrentlyPlayingSample> listActiveSamples;

public:
	AudioEngine(
		audio_handler fSoundSample = nullptr,
		audio_handler fSoundFilter = nullptr
	); ~AudioEngine(void);

	int LoadAudioSample(std::wstring sWavFile);

	void PlaySample(int id, bool bLoop = false);
	void StopSample(int id);

	bool CreateAudio(
		unsigned int nSampleRate = 44100, unsigned int nChannels = 0x1,
		unsigned int nBlocks = 0x8, unsigned int nBlockSamples = 512
	);
	bool DestroyAudio(void);

	float GetGlobalTime(void);

private:
	void waveOutProc(HWAVEOUT hWaveOut,
		UINT uMsg, DWORD dwParam1, DWORD dwParam2);

	static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut,
		UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2
	);

	void AudioThread(void);

	float GetMixerOutput(int nChannel,
		float fGlobalTime, float fTimeStep);
};

#endif //_AUDIO_ENGINE_H_
