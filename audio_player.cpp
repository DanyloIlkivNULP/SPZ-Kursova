#include "audio_player.h"

#include "audio_sample.h"
#include "playing_audio.h"

AudioEngine::AudioPlayer::AudioPlayer(AUDIOID nAudioSampleID,
	pAudioEngine pAE) : m_pAE(pAE)
{
	m_sData.m_pAS = m_pAE.get()->
		vecAudioSamples[nAudioSampleID - 0x1];

	m_sData.m_pCPA = std::make_shared
		<PlayingAudio>(nAudioSampleID);
	m_pAE.get()->listActiveSamples.
		push_back(std::move(m_sData.m_pCPA));
}
AudioEngine::AudioPlayer::~AudioPlayer(void) { /*Code...*/ }
