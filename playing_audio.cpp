#include "playing_audio.h"

#include "audio_sample.h"

AudioEngine::PlayingAudio::PlayingAudio(AUDIOID nAudioSampleID) :
	m_nAudioSampleID(nAudioSampleID)
{
	m_fSamplePosition = 0.f;
	m_bFinish = false;
}
AudioEngine::PlayingAudio::~PlayingAudio(void)
{ /*Code...*/ }

AUDIOID AudioEngine::PlayingAudio::AudioSampleID(void) const
{ return(m_nAudioSampleID); }
bool AudioEngine::PlayingAudio::IsFinish(void) const
{ return(m_bFinish); }

float AudioEngine::PlayingAudio::ProcessAudioSample(int nChannel,
	float fGlobalTime, float fTimeStep, float fMixerSample,
		const std::shared_ptr<AudioEngine::AudioSample>& pS
)
{
	// Calculate sample position
	m_fSamplePosition.store(
		m_fSamplePosition.load() + (float)pS->wavHeader.nSamplesPerSec * fTimeStep
	);
	
	// If sample position is valid add to the mix
	if (m_fSamplePosition < pS.get()->m_nSamples)
	{ fMixerSample += pS.get()->m_fSample[((long)round(m_fSamplePosition) *
		pS.get()->m_nChannels) + nChannel];
	}
	else
	{ m_bFinish = true; } // Else sound has completed
	return(fMixerSample);
}
