#include "playing_audio_sample.h"

#include "audio_sample.h"

AudioEngine::PlayingAudioSample::PlayingAudioSample(int nAudioSampleID) :
	m_nAudioSampleID(nAudioSampleID)
{
	m_fSamplePosition = 0.f;
	m_bFinish = false;
}
AudioEngine::PlayingAudioSample::~PlayingAudioSample(void)
{ /*Code...*/ }

int AudioEngine::PlayingAudioSample::AudioSampleID(void) const
{ return(m_nAudioSampleID); }
bool AudioEngine::PlayingAudioSample::IsFinish(void) const
{ return(m_bFinish); }

float AudioEngine::PlayingAudioSample::ProcessAudioSample(int nChannel,
	float fGlobalTime, float fTimeStep, float fMixerSample,
		const AudioEngine::AudioSample* const pS
)
{
	// Calculate sample position
	m_fSamplePosition += (float)pS->wavHeader.nSamplesPerSec * fTimeStep;
	
	// If sample position is valid add to the mix
	if (m_fSamplePosition < pS->m_nSamples)
	{ fMixerSample += pS->
		m_fSample[((long)round(m_fSamplePosition) * pS->m_nChannels) + nChannel];
	}
	else
	{ m_bFinish = true; } // Else sound has completed
	return(fMixerSample);
}
