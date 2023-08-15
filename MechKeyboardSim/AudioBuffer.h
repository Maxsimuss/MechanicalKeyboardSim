#pragma once
#include <AL/al.h>
#include <AL/alext.h>
#include <stdint.h>
#include "AudioData.h"
#include "AudioSourceConfig.h"

class AudioBuffer {
private:
	AudioSourceConfig audioSourceConfig;

	uint32_t buffer = 0;
	uint32_t sampleRate;
	double length;

public:
	AudioBuffer(AudioData data, int sampleRate, AudioSourceConfig audioSourceConfig)
		: sampleRate(sampleRate),
		length(data.sampleCount / data.sampleRate),
		audioSourceConfig(audioSourceConfig) {
		alGenBuffers(1, &buffer);

		if (data.channels == 1) {
			alBufferData(buffer, AL_FORMAT_MONO_FLOAT32, data.data, data.sampleCount * data.channels * sizeof(float), sampleRate);
		}
		else if (data.channels == 2) {
			alBufferData(buffer, AL_FORMAT_STEREO_FLOAT32, data.data, data.sampleCount * data.channels * sizeof(float), sampleRate);
		}
		else {
			throw data.channels;
		}
	}

	~AudioBuffer() {
		alDeleteBuffers(1, &buffer);
	}

	const uint32_t& getBuffer() const {
		return buffer;
	}

	const uint32_t& getSamplerate() const {
		return sampleRate;
	}

	const double& getLength() const {
		return length;
	}
};

