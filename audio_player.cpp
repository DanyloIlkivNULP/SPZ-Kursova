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

void AudioPlayer::SwapStateAudio(void)
{ m_bState = !m_bState; }
void AudioPlayer::ChangeStateAudio(bool bState)
{ m_bState = bState; }
bool AudioPlayer::CurrentStateAudio(void) const
{ return(m_bState.load()); }



void AudioPlayer::VolumeAudio(float fVolume) {
	fVolume = Clip(fVolume);
	m_fVolume.store(fVolume);
}
float AudioPlayer::CurrentVolume(void) const
{ return(m_fVolume.load()); }

void AudioPlayer::PitchAudio(float fPitch) {
	fPitch = fmax(fPitch, 0.f);
	m_fPitch.store(fPitch);
}
float AudioPlayer::CurrentPitch(void) const
{ return(m_fPitch.load()); }



void AudioPlayer::PositonAudio(double dSamplePosition) {
	dSamplePosition = Clip(dSamplePosition);
	m_pCPA.get()->m_dSamplePosition = (double)m_pAS.get()->
		m_nSamples * dSamplePosition;
}

double AudioPlayer::CurrentPositonAudio(void) const
{ return(m_pCPA.get()->m_dSamplePosition / m_pAS.get()->m_nSamples); }

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
	if (m_bState != STATE_PLAY) {
		m_pCPA.get()->m_dSamplePosition.store(m_pCPA.get()->m_dSamplePosition.load() + 
			(double)pS->wavHeader.nSamplesPerSec * m_fPitch.load() * fTimeStep
		);
	} else
	{ return(fMixerSample); }

	// If sample position is valid add to the mix
	if (m_pCPA.get()->m_dSamplePosition < pS->m_nSamples) {
		fMixerSample += (
			pS->m_fSample[((long)round(m_pCPA.get()->m_dSamplePosition) * pS->m_nChannels) + nChannel]
		) * m_fVolume.load();
	}
	else {
		m_bState = STATE_STOP;
		m_pCPA.get()->m_dSamplePosition = 0.0;
	} // Else sound has completed
	return(fMixerSample);
}



AudioPlayer::ActivePlayingAudio::ActivePlayingAudio(AUDIOID nAudioSampleID,
	pAudioPlayer pAP) : AudioEngine::PlayingAudio(nAudioSampleID), m_pAP(pAP)
{
	m_dSamplePosition = 0.0;
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

float AudioPlayer::Clip(float& f) {
	if (f >= 0.f)
	{ f = fmin(f, 1.f); }
	else
	{ f = fmax(f, 0.f); }
	return(f);
}

double AudioPlayer::Clip(double& d) {
	if (d >= 0.f)
	{ d = fmin(d, 1.0); }
	else
	{ d = fmax(d, 0.0); }
	return(d);
}
