#include "audio_player.h"

#include "audio_sample.h"

AudioPlayer::AudioPlayer(pAudioEngine pAE,
	AUDIOID nAudioSampleID) : m_pAE(pAE)
{
	std::lock_guard<std::mutex>
		lgProcessAudio(m_pAE->m_muxProcessAudio);

	m_pAE->CreatePlayingAudio
		<ActivePlayingAudio>(nAudioSampleID, this);
	m_pCPA = m_pAE->listActiveSamples.back();
}
AudioPlayer::~AudioPlayer(void) { /*Code...*/ }

void AudioPlayer::PauseAudio(void) {
	std::lock_guard<std::mutex>
		lgProcessAudio(m_pAE->m_muxProcessAudio);
	m_bPause = !m_bPause;
}

AudioPlayer::ActivePlayingAudio::ActivePlayingAudio(AUDIOID nAudioSampleID,
	pAudioPlayer pAP) : AudioEngine::PlayingAudio(nAudioSampleID), m_pAP(pAP)
{
	m_fSamplePosition = 0.f;
	m_bFinish = false;
}
AudioPlayer::ActivePlayingAudio::~ActivePlayingAudio(void) { /*Code...*/ }

float AudioPlayer::ActivePlayingAudio::ProcessAudioSample(int nChannel,
	float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
)
{
	// Calculate sample position
	if (!m_pAP->m_bPause)
	{ m_fSamplePosition +=
		(float)pS->wavHeader.nSamplesPerSec * fTimeStep;
	}
	else { goto linkExit; }
	
	// If sample position is valid add to the mix
	if (m_fSamplePosition < pS->m_nSamples)
	{ fMixerSample += pS->
		m_fSample[((long)round(m_fSamplePosition) * pS->m_nChannels) + nChannel];
	}
	else
	{ m_fSamplePosition = 0.f; } // Else sound has completed
linkExit:
	return(fMixerSample);
}
