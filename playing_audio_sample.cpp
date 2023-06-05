#include "playing_audio_sample.h"

#include "audio_sample.h"

AudioEngine::PlayingAudioSample::PlayingAudioSample(int nAudioSampleID) :
	m_nAudioSampleID(nAudioSampleID)
{
	m_fSamplePosition = 0.f;
	m_bFinished = false;
}
AudioEngine::PlayingAudioSample::~PlayingAudioSample(void)
{ /*Code...*/ }

int AudioEngine::PlayingAudioSample::GetAudioSampleID(void)
{ return(m_nAudioSampleID); }

float AudioEngine::PlayingAudioSample::ProcessAudioSample(int nChannel,
	float fGlobalTime, float fTimeStep, float fMixerSample,
		const AudioEngine::AudioSample* const pS
)
{
	// Calculate sample position
	m_fSamplePosition += (float)pS->wavHeader.nSamplesPerSec * fTimeStep;
	
	// If sample position is valid add to the mix
	if (m_fSamplePosition < pS->nSamples)
	{ fMixerSample += pS->
		fSample[((long)round(m_fSamplePosition) * pS->nChannels) + nChannel];
	}
	else
	{ m_bFinished = true; } // Else sound has completed
	return(fMixerSample);
}
