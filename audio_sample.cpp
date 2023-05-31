#include "audio_sample.h"

#include "logger.h"

AudioEngine::AudioSample::AudioSample(void)
{ /*Code...*/ }
AudioEngine::AudioSample::~AudioSample(void)
{ /*Code...*/ }

AudioEngine::AudioSample::AudioSample(std::wstring sWavFile) {
	// Load Wav file and convert to float format
	FILE* f = nullptr;
	_wfopen_s(&f, sWavFile.c_str(), L"rb");
	if (f == nullptr) { return; }

	char dump[0x4] = { 0x0 };
	std::fread(&dump, sizeof(char), 0x4, f); // Read "RIFF"
	if (strncmp(dump, "RIFF", 0x4) != 0x0) { return; }
	std::fread(&dump, sizeof(char), 0x4, f); // Not Interested
	std::fread(&dump, sizeof(char), 0x4, f); // Read "WAVE"
	if (strncmp(dump, "WAVE", 0x4) != 0x0) { return; }

	// Read Wave description chunk
	std::fread(&dump, sizeof(char), 0x4, f); // Read "FMT"
	std::fread(&dump, sizeof(char), 0x4, f); // Not Interested
	// Read Wave Format Structure chunk
	// Note the -2, because the structure has 2 bytes to indicate its own size
	// which are not in the wav file
	std::fread(&wavHeader, sizeof(WAVEFORMATEX) - 0x2, 0x1, f);

	// Just check if wave format is compatible with AE
	if (wavHeader.wBitsPerSample != 0x10 || wavHeader.nSamplesPerSec != 44100)
	{ std::fclose(f); return; }

	// Search for audio data chunk
	long nChunksize = 0x0;
	std::fread(&dump, sizeof(char), 0x4, f); // Read chunk header
	std::fread(&nChunksize, sizeof(long), 0x1, f); // Read chunk size
	while (strncmp(dump, "data", 0x4) != 0x0) {
		// Not audio data, so just skip it
		std::fseek(f, nChunksize, SEEK_CUR);
		std::fread(&dump, sizeof(char), 0x4, f);
		std::fread(&nChunksize, sizeof(long), 0x1, f);
	}

	// Finally got to data, so read it all in and convert to float samples
	nSamples = nChunksize / (wavHeader.nChannels * (wavHeader.wBitsPerSample >> 0x3));
	nChannels = wavHeader.nChannels;

	// Create floating point buffer to hold audio sample
	fSample = new float[nSamples * nChannels];
	float* pSample = fSample;

	// Read in audio data and normalise
	for (long i = 0x0; i < nSamples; i++)
		for (int c = 0x0; c < nChannels; c++) {
			short s = 0x0;
			std::fread(&s, sizeof(short), 0x1, f);
			*pSample = (float)s / (float)(MAXSHORT);
			pSample += 0x1;
		}

	// All done, flag sound as valid
	std::fclose(f);
	bSampleValid = true;
}