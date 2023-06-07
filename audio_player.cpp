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
	m_pCPA.get()->m_bFinish = true;
}

const wchar_t*
	AudioPlayer::FileName(void) const
{ return(m_pAS.get()->FileName()); }

void AudioPlayer::PauseAudio(void)
{ m_bPause = !m_bPause; }
void AudioPlayer::PauseAudio(bool bState)
{ m_bPause = bState; }

void AudioPlayer::VolumeAudio(float fVolume) {
	if (fVolume >= 0.f)
	{ fVolume = fmin(fVolume, 1.f); }
	else
	{ fVolume = fmax(fVolume, 0.f); }
	m_fVolume.store(fVolume);
}
float AudioPlayer::CurrentVolume(void) const
{ return(m_fVolume); }

void AudioPlayer::PositonAudio(float fSamplePosition) {
	if (fSamplePosition >= 0.f)
	{ fSamplePosition = fmin(fSamplePosition, 1.f); }
	else
	{ fSamplePosition = fmax(fSamplePosition, 0.f); }
	m_pCPA.get()->m_fSamplePosition = m_pAS.get()->
		m_nSamples * fSamplePosition;
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
	if (!m_bPause) {
		m_pCPA.get()->m_fSamplePosition.store(m_pCPA.get()->m_fSamplePosition.load() + 
			(float)pS->wavHeader.nSamplesPerSec * fTimeStep
		);
	} else
	{ return(fMixerSample); }

	// If sample position is valid add to the mix
	if (m_pCPA.get()->m_fSamplePosition < pS->m_nSamples) {
		fMixerSample += (
			pS->m_fSample[((long)round(m_pCPA.get()->m_fSamplePosition) * pS->m_nChannels) + nChannel]
		) * m_fVolume.load();
	}
	else {
		m_bPause = true;
		m_pCPA.get()->m_fSamplePosition = 0.f;
	} // Else sound has completed
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
