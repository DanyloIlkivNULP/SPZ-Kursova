#include "audio_player.h"

#include "audio_sample.h"

AudioPlayer::AudioPlayer(pAudioEngine pAE,
	AUDIOID nAudioSampleID) : m_pAE(pAE)
{
	/*Code...*/
}
AudioPlayer::~AudioPlayer(void)
{ /*Code...*/ }

AudioPlayer::AUDIO_DATA AudioPlayer::CreateAudio
	(AUDIOID nAudioSampleID)
{
	std::lock_guard<std::mutex>
		lgProcessAudio(m_pAE->m_muxProcessAudio);

	m_pAE->CreatePlayingAudio
		<ActivePlayingAudio>(nAudioSampleID, this);
	return(AUDIO_DATA(
		m_pAE->vecAudioSamples[nAudioSampleID - 0x1],
		m_pAE->listActiveSamples.back()
	));
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
