#include "WaveFileReader.h"

// modified from https://stackoverflow.com/a/11162668
AudioData WaveFileReader::parseSamples() {
	int channelCount = data[22];
	int bytesPerSample = (*(int16_t*)&data[34]) / 8;
	int sampleRate = *((int*)&data[24]);

	uint32_t pos = 12;   // First Subchunk ID from 12 to 16

	// Keep iterating until we find the data chunk (i.e. 64 61 74 61 ...... (i.e. 100 97 116 97 in decimal))
	while (!(data[pos] == 100 && data[pos + 1] == 97 && data[pos + 2] == 116 && data[pos + 3] == 97)) {
		pos += 4;
		int chunkSize = data[pos] + data[pos + 1] * 256 + data[pos + 2] * 65536 + data[pos + 3] * 16777216;
		pos += 4 + chunkSize;
	}
	pos += 8;

	// Pos is now positioned to start of actual sound data.
	uint32_t sampleCount = (dataLength - pos) / bytesPerSample / channelCount;

	float* samples = new float[sampleCount * channelCount];

	int i = 0;
	if (bytesPerSample == 1) {
		while (pos < dataLength) {
			samples[i++] = ((int8_t)data[pos]) / INT8_MAX - .5f;
			pos += bytesPerSample;
		}
	}
	else if (bytesPerSample == 2) {
		while (pos < dataLength) {
			samples[i++] = (float)(*(int16_t*)&data[pos]) / INT16_MAX;
			pos += bytesPerSample;
		}
	}
	else if (bytesPerSample == 3) {
		while (pos < dataLength) {
			uint32_t part0 = *(uint8_t*)&data[pos];
			uint32_t part1 = *(uint8_t*)&data[pos + 1];
			uint32_t part2 = *(uint8_t*)&data[pos + 2];

			int32_t assembled = part0 << 8 | part1 << 16 | part2 << 24;


			samples[i++] = (float)assembled / (float)INT32_MAX;
			pos += bytesPerSample;
		}
	}
	else if (bytesPerSample == 4) {
		while (pos < dataLength) {
			samples[i++] = *((float*)&data[pos]);
			pos += bytesPerSample;
		}
	}
	else {
		throw std::runtime_error("Wave file format not supported!");
	}

	AudioData audioData(channelCount, sampleCount, sampleRate, samples);

	return audioData;
}