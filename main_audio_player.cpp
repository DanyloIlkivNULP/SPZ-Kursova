#include "main_audio_player.h"

#include "audio_engine.h"
#include "audio_sample.h"
#include "playing_audio.h"

MainAudioPlayer::MainAudioPlayer(pAudioEngine pAE,
	AUDIOID nAudioSampleID) : AudioPlayer(pAE, nAudioSampleID)
{
	AUDIO_DATA ad = CreateAudio
		(nAudioSampleID);

	m_vecAudio.push_back(ad);
	m_nCurrentAudio = m_vecAudio.size();
}
MainAudioPlayer::~MainAudioPlayer(void)
{
	AudioData().second.get()->m_bFinish = true;
}

AudioPlayer::AUDIO_DATA& MainAudioPlayer::AudioData(void) const
{ return(m_vecAudio[m_nCurrentAudio - 0x1]); }

std::shared_ptr<AudioEngine::AudioSample>&
	MainAudioPlayer::AudioSample(void) const
{ return(m_vecAudio[m_nCurrentAudio - 0x1].first); }
std::shared_ptr<AudioEngine::PlayingAudio>&
	MainAudioPlayer::PlayingAudio(void) const
{ return(m_vecAudio[m_nCurrentAudio - 0x1].second); }



const wchar_t*MainAudioPlayer::FileName(void) const
{ return(AudioSample().get()->m_wcWavFile); }

void MainAudioPlayer::SwapStateAudio(void)
{ m_bState = !m_bState; }
void MainAudioPlayer::ChangeStateAudio(bool bState)
{ m_bState = bState; }
bool MainAudioPlayer::CurrentStateAudio(void) const
{ return(m_bState.load()); }



void MainAudioPlayer::VolumeAudio(float fVolume) {
	fVolume = Clip(fVolume);
	m_fVolume.store(fVolume);
}
float MainAudioPlayer::CurrentVolume(void) const
{ return(m_fVolume.load()); }

void MainAudioPlayer::PitchAudio(float fPitch) {
	fPitch = fmax(fPitch, 0.f);
	m_fPitch.store(fPitch);
}
float MainAudioPlayer::CurrentPitch(void) const
{ return(m_fPitch.load()); }



void MainAudioPlayer::PositonAudio(double dSamplePosition) {
	dSamplePosition = Clip(dSamplePosition);
	PlayingAudio().get()->m_dSamplePosition = (double)AudioSample().get()->
		m_nSamples * dSamplePosition;
}

double MainAudioPlayer::CurrentPositonAudio(void) const
{ return(PlayingAudio().get()->m_dSamplePosition / AudioSample().get()->m_nSamples); }

DWORD MainAudioPlayer::NumOfSamples(void) const
{ return(AudioSample().get()->m_nSamples); }

DWORD MainAudioPlayer::NumOfSamplesPerSec(void) const
{ return(AudioSample().get()->wavHeader.nSamplesPerSec); }



float MainAudioPlayer::AudioHandler(int nChannel,
	float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
)
{
	// Calculate sample position
	if (m_bState != STATE_PLAY) {
		PlayingAudio().get()->m_dSamplePosition.store(PlayingAudio().get()->m_dSamplePosition.load() +
			(double)pS->wavHeader.nSamplesPerSec * m_fPitch.load() * fTimeStep
		);
	} else
	{ return(fMixerSample); }

	// If sample position is valid add to the mix
	if (PlayingAudio().get()->m_dSamplePosition < pS->m_nSamples) {
		fMixerSample += (
			pS->m_fSample[((long)round(PlayingAudio().get()->m_dSamplePosition) * pS->m_nChannels) + nChannel]
		) * m_fVolume.load();
	}
	else {
		m_bState = STATE_STOP;
		PlayingAudio().get()->m_dSamplePosition = 0.0;
	} // Else sound has completed
	return(fMixerSample);
}

float MainAudioPlayer::Clip(float& f) {
	if (f >= 0.f)
	{ f = fmin(f, 1.f); }
	else
	{ f = fmax(f, 0.f); }
	return(f);
}

double MainAudioPlayer::Clip(double& d) {
	if (d >= 0.f)
	{ d = fmin(d, 1.0); }
	else
	{ d = fmax(d, 0.0); }
	return(d);
}
