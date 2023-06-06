#include "audio_player.h"

#include "audio_sample.h"

AudioPlayer::AudioPlayer(pAudioEngine pAE,
	AUDIOID nAudioSampleID) : m_pAE(pAE)
{
	std::lock_guard<std::mutex>
		lgProcessAudio(m_pAE->m_muxProcessAudio);

	m_pAS = m_pAE->
		vecAudioSamples[nAudioSampleID - 0x1];

	m_pAE->CreatePlayingAudio
		<ActivePlayingAudio>(nAudioSampleID, this);
	m_pCPA = m_pAE->listActiveSamples.back();
}
AudioPlayer::~AudioPlayer(void) {
	std::lock_guard<std::mutex>
		lgProcessAudio(m_pAE->m_muxProcessAudio);
	m_pCPA.get()->m_bFinish = true;
}

void AudioPlayer::PauseAudio(void) {
	std::lock_guard<std::mutex>
		lgProcessAudio(m_pAE->m_muxProcessAudio);
	m_bPause = !m_bPause;
}
void AudioPlayer::PauseAudio(bool bState) {
	std::lock_guard<std::mutex>
		lgProcessAudio(m_pAE->m_muxProcessAudio);
	m_bPause = bState;
}

void AudioPlayer::PositonAudio(float fSamplePosition) {
	std::lock_guard<std::mutex>
		lgProcessAudio(m_pAE->m_muxProcessAudio);
	m_pCPA.get()->m_fSamplePosition = m_pAS.get()->m_nSamples * fSamplePosition;
}

float AudioPlayer::CurrentPositonAudio(void) const
{ return(m_pCPA.get()->m_fSamplePosition / m_pAS.get()->m_nSamples); }

DWORD AudioPlayer::NumOfSamples(void) const
{ return(m_pAS.get()->m_nSamples); }

DWORD AudioPlayer::NumOfSamplesPerSec(void) const
{ return(m_pAS.get()->wavHeader.nSamplesPerSec); }



float AudioPlayer::AudioHandler(int nChannel,
	float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
)
{
	// Calculate sample position
	if (!m_bPause)
	{
		m_pCPA.get()->m_fSamplePosition +=
			(float)pS->wavHeader.nSamplesPerSec * fTimeStep;
	}
	else { goto linkExit; }

	// If sample position is valid add to the mix
	if (m_pCPA.get()->m_fSamplePosition < pS->m_nSamples)
	{
		fMixerSample += pS->
			m_fSample[((long)round(m_pCPA.get()->m_fSamplePosition) * pS->m_nChannels) + nChannel];
	}
	else
	{
		m_bPause = true;
		m_pCPA.get()->m_fSamplePosition = 0.f;
	} // Else sound has completed
linkExit:
	return(fMixerSample);
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
	float fResult = 0.f; 
	if (m_bFinish) { goto linkExit; }

	fResult = m_pAP->AudioHandler
		(nChannel, fGlobalTime, fTimeStep, fMixerSample, pS);
linkExit:
	return(fResult);
}
