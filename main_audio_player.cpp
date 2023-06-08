#include "main_audio_player.h"

#include "audio_engine.h"
#include "audio_sample.h"
#include "playing_audio.h"

MainAudioPlayer::MainAudioPlayer(pAudioEngine pAE) :
	AudioPlayer(pAE)
{
	/*Code...*/
}
MainAudioPlayer::~MainAudioPlayer(void)
{
	/*Code...*/
}

AudioPlayer::AUDIO_DATA&
	MainAudioPlayer::AudioData(void) const
{
	return(m_vecAudio
		[m_nCurrentAudio.load() - 0x1]
	);
}

std::shared_ptr<AudioEngine::AudioSample>&
	MainAudioPlayer::AudioSample(void) const
{
	return(m_vecAudio
		[m_nCurrentAudio.load() - 0x1].first
	);
}
std::shared_ptr<AudioEngine::PlayingAudio>&
	MainAudioPlayer::PlayingAudio(void) const
{
	return(m_vecAudio
		[m_nCurrentAudio.load() - 0x1].second
	);
}

AUDIOID MainAudioPlayer::LoadAudio(AUDIOID ID) {
	if (ID == -0x1)
	{ return(ID); }

	std::lock_guard<std::recursive_mutex>
		lgData(m_muxData);

	AUDIO_DATA ad = CreateAudio(ID);

	m_vecAudio.push_back(ad);
	m_nCurrentAudio.store
		(m_vecAudio.size());

	m_bState.store(STATE_STOP);
	return(m_nCurrentAudio.load());
}

bool MainAudioPlayer::ChangeCurrentAudio
	(AUDIOID ID)
{
	bool bResult = 0x0;
	std::lock_guard<std::recursive_mutex>
		lgData(m_muxData);

	if (ID < 0x0) { return(bResult); }
	if (bResult = ((size_t)ID <= m_vecAudio.size()))
		{ m_nCurrentAudio.store(ID); }
	return(bResult);
}

AUDIOID MainAudioPlayer::CurrentAudio(void) const
{ return(m_nCurrentAudio.load()); }

const wchar_t* MainAudioPlayer::FileName(void) const {
	if (m_nCurrentAudio.load() == -0x1) { return(NULL); }

	std::lock_guard<std::recursive_mutex>
		lgData(m_muxData);

	return(AudioSample().get()->
		m_wsWavFile.data());
}

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
	if (m_nCurrentAudio.load() == -0x1) { return; }
	std::lock_guard<std::recursive_mutex>
		lgData(m_muxData);

	dSamplePosition = Clip(dSamplePosition);
	PlayingAudio().get()->m_dSamplePosition = (double)AudioSample().get()->
		m_nSamples * dSamplePosition;
}

double MainAudioPlayer::CurrentPositonAudio(void) const {
	if (m_nCurrentAudio.load() == -0x1) { return(0.0); }
	std::lock_guard<std::recursive_mutex>
		lgData(m_muxData);

	return(PlayingAudio().get()->m_dSamplePosition /
		AudioSample().get()->m_nSamples
	);
}

DWORD MainAudioPlayer::NumOfSamples(void) const {
	if (m_nCurrentAudio.load() == -0x1) { return(0x0); }
	std::lock_guard<std::recursive_mutex>
		lgData(m_muxData);

	return(AudioSample().get()->
		m_nSamples
	);
}

DWORD MainAudioPlayer::NumOfSamplesPerSec(void) const {
	if (m_nCurrentAudio.load() == -0x1) { return(0x0); }
	std::lock_guard<std::recursive_mutex>
		lgData(m_muxData);

	return(AudioSample().get()->
		wavHeader.nSamplesPerSec
	);
}



float MainAudioPlayer::AudioHandler(int nChannel,
	float fGlobalTime, float fTimeStep, float fMixerSample,
		const pAudioSample pS, const pPlayingAudio pA
)
{
	std::lock_guard<std::recursive_mutex>
		lgData(m_muxData);

	if (PlayingAudio().get() != pA)
		{ return(fMixerSample); }
	// Calculate sample position
	if (m_bState.load() != STATE_STOP) {
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
		PlayingAudio().get()->
			m_dSamplePosition = (double)pS->m_nSamples;
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
