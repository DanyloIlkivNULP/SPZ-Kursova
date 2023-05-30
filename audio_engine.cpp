#include "audio_engine.h"

#include "audio_sample.h"

#include "logger.h"

AudioEngine::AudioEngine(void) {
	(void)Logger::LogMessage
		(Logger::LogLevel::LOG_LVL_INFO,
			L"[Function] : " __FUNCTION__ L" - Success!\t[File] : " __FILE__);
}
AudioEngine::~AudioEngine(void) {
	(void)Logger::LogMessage
		(Logger::LogLevel::LOG_LVL_INFO,
			L"[Function] : " __FUNCTION__ L" - Success!\t[File] : " __FILE__);
}

// Load a 16-bit WAVE file @ 44100Hz ONLY into memory. A sample ID
// number is returned if successful, otherwise -1
int AudioEngine::LoadAudioSample(std::wstring sWavFile) {
	AudioSample a(sWavFile);
	if (a.bSampleValid) {
		vecAudioSamples.push_back(a);
		return(vecAudioSamples.size());
	}
	else
	{ return(-0x1); }
}

// Add sample 'id' to the mixers sounds to play list
void AudioEngine::PlaySample(int id, bool bLoop) {
	sCurrentlyPlayingSample s;
	s.nAudioSampleID = id;
	s.nSamplePosition = 0x0;
	s.bLoop = bLoop;

	s.bFinished = false;
	listActiveSamples.push_back(s);
}

void AudioEngine::StopSample(int id) { /*Code...*/ }

// The audio system uses by default a specific wave format
bool AudioEngine::CreateAudio(unsigned int nSampleRate, unsigned int nChannels,
	unsigned int nBlocks, unsigned int nBlockSamples) {
	// Initialise Sound Engine
	m_bAudioThreadActive = false;

	m_nSampleRate = nSampleRate;
	m_nChannels = nChannels;
	m_nBlockCount = nBlocks;
	m_nBlockSamples = nBlockSamples;
	m_nBlockFree = m_nBlockCount;
	m_nBlockCurrent = 0x0;

	m_pBlockMemory = nullptr;
	m_pWaveHeaders = nullptr;

	// Device is available
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = m_nSampleRate;
	waveFormat.wBitsPerSample = sizeof(short) * 0x8;
	waveFormat.nChannels = m_nChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 0x8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0x0;

	// Open Device if valid
	if (waveOutOpen(&m_hwDevice, WAVE_MAPPER,
		&waveFormat, (DWORD_PTR)waveOutProcWrap,
		(DWORD_PTR)this, CALLBACK_FUNCTION) != S_OK
	)
	{ return(DestroyAudio()); }

	// Allocate Wave|Block Memory
	m_pBlockMemory = new short[m_nBlockCount * m_nBlockSamples];
	if (m_pBlockMemory == nullptr)
	{ return(DestroyAudio()); }
	ZeroMemory(m_pBlockMemory, sizeof(short) * m_nBlockCount * m_nBlockSamples);

	m_pWaveHeaders = new WAVEHDR[m_nBlockCount];
	if (m_pWaveHeaders == nullptr)
	{ return(DestroyAudio()); }
	ZeroMemory(m_pWaveHeaders, sizeof(WAVEHDR) * m_nBlockCount);

	// Link headers to block memory
	for (unsigned int n = 0x0; n < m_nBlockCount; n++) {
		m_pWaveHeaders[n].dwBufferLength = m_nBlockSamples * sizeof(short);
		m_pWaveHeaders[n].lpData = (LPSTR)(m_pBlockMemory + (n * m_nBlockSamples));
	}

	m_bAudioThreadActive = true;
	m_AudioThread = std::thread(&AudioEngine::AudioThread, this);

	return(true);
}

// Stop and clean up audio system
bool AudioEngine::DestroyAudio(void) {
	if (listActiveSamples.size() != 0x0) {
		std::unique_lock<std::mutex> lm(m_muxActiveSamplesEmpty);
		while (listActiveSamples.size() != 0x0)
		{ m_cvActiveSamplesEmpty.wait(lm); }
	}

	m_bAudioThreadActive = false;
	m_AudioThread.join();

	return(false);
}

// Handler for soundcard request for more data
void AudioEngine::waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2) {
	switch (uMsg)
	{
	case WOM_OPEN:
	{ /*Code...*/ } break;
	case WM_CLOSE:
	{ /*Code...*/ } break;

	case WOM_DONE: {
		m_nBlockFree += 0x1;
		std::unique_lock<std::mutex> lm(m_muxBlockNotZero);
		m_cvBlockNotZero.notify_one();
	} break;

	default:
		break;
	}
}

// Static wrapper for sound card handler
void CALLBACK AudioEngine::waveOutProcWrap(HWAVEOUT hWaveOut,
	UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2
)
{
	((AudioEngine*)dwInstance)->
		waveOutProc(hWaveOut, uMsg, dwParam1, dwParam2);
}

// Audio thread. This loop responds to requests from the soundcard to fill 'blocks'
// with audio data. If no requests are available it goes dormant until the sound
// card is ready for more data. The block is fille by the "user" in some manner
// and then issued to the soundcard.
void AudioEngine::AudioThread(void) {
	m_fGlobalTime = 0.f;
	float fTimeStep = 1.f / (float)m_nSampleRate;

	// Goofy hack to get maximum integer for a type at run-time
	short nMaxSample = (short)pow(0x2, (sizeof(short) * 0x8) - 0x1) - 0x1;
	float fMaxSample = (float)nMaxSample;
	short nPreviousSample = 0x0;

	while (m_bAudioThreadActive) {
		// Wait for block to become available
		if (m_nBlockFree == 0x0) {
			std::unique_lock<std::mutex> lm(m_muxBlockNotZero);
			while (m_nBlockFree == 0x0)
			{ m_cvBlockNotZero.wait(lm); }
		}

		// Block is here, so use it
		m_nBlockFree -= 0x1;

		// Prepare block for processing
		if (m_pWaveHeaders[m_nBlockCurrent].dwFlags & WHDR_PREPARED)
		{ waveOutUnprepareHeader(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR)); }

		short nNewSample = 0x0;
		int nCurrentBlock = m_nBlockCurrent * m_nBlockSamples;

		auto clip = [](float fSample, float fMax) {
			if (fSample >= 0.f)
			{ return(fmin(fSample, fMax)); }
			else
			{ return(fmax(fSample, -fMax)); }
		};

		for (unsigned int n = 0x0; n < m_nBlockSamples; n += m_nChannels) {
			// User Process
			for (unsigned int c = 0x0; c < m_nChannels; c++) {
				nNewSample = (short)(clip(GetMixerOutput(c, m_fGlobalTime, fTimeStep), 1.f) * fMaxSample);
				m_pBlockMemory[nCurrentBlock + n + c] = nNewSample;
				nPreviousSample = nNewSample;
			}

			m_fGlobalTime = m_fGlobalTime + fTimeStep;
		}

		// Send block to sound device
		waveOutPrepareHeader(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR));
		waveOutWrite(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR));

		m_nBlockCurrent += 0x1;
		m_nBlockCurrent %= m_nBlockCount;
	}
}



// The Sound Mixer - If the user wants to play many sounds simultaneously, and
// perhaps the same sound overlapping itself, then you need a mixer, which
// takes input from all sound sources for that audio frame. This mixer maintains
// a list of sound locations for all concurrently playing audio samples. Instead
// of duplicating audio data, we simply store the fact that a sound sample is in
// use and an offset into its sample data. As time progresses we update this offset
// until it is beyound the length of the sound sample it is attached to. At this
// point we remove the playing souind from the list.
float AudioEngine::GetMixerOutput(int nChannel, float fGlobalTime, float fTimeStep) {
	// Accumulate sample for this channel
	float fMixerSample = 0.f;

	for (auto& s : listActiveSamples) {
		// Calculate sample position
		s.nSamplePosition += (long)((float)vecAudioSamples[s.nAudioSampleID - 0x1].wavHeader.nSamplesPerSec * fTimeStep);

		// If sample position is valid add to the mix
		if (s.nSamplePosition < vecAudioSamples[s.nAudioSampleID - 0x1].nSamples)
		{ fMixerSample += vecAudioSamples[s.nAudioSampleID - 0x1].
			fSample[(s.nSamplePosition * vecAudioSamples[s.nAudioSampleID - 0x1].nChannels) + nChannel];
		}
		else
		{ s.bFinished = true; } // Else sound has completed 
	}

	// If sounds have completed then remove them
	listActiveSamples.remove_if([](const sCurrentlyPlayingSample& s)
		{ return(s.bFinished); }
	);

	if (listActiveSamples.empty()) {
		std::unique_lock<std::mutex>
			lm(m_muxActiveSamplesEmpty);
		m_cvActiveSamplesEmpty.notify_one();
	}

	// Return the sample
	return(fMixerSample);
}
