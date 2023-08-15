#pragma once
#include "AudioBuffer.h"

class SoundSource {
public:
	bool isPlaying;

	AudioBuffer* buffer;

	float pitch, volume;
	uint32_t source;

	SoundSource(AudioBuffer* buffer, float pitch, float volume) {
		this->buffer = buffer;
		this->pitch = pitch;
		this->volume = volume;
	}

	void play() {
		if (isPlaying) return;

		alGenSources(1, &source);

		alSourcei(source, AL_BUFFER, buffer->getBuffer());
		alSourcei(source, AL_LOOPING, AL_FALSE);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcef(source, AL_GAIN, volume);
		alSourcef(source, AL_PITCH, pitch);


		//alSourcei(source, AL_SAMPLE_OFFSET, (int)(Math.Min(buffer.length - 1d / buffer.sampleRate, buffer.length - Math.Max(0.0001, TTL)) * buffer.sampleRate));

		alSourcePlay(source);

		isPlaying = true;
	}

	void stop() {
		if (!isPlaying) return;

		alSourceStop(source);
		alDeleteSources(1, &source);

		isPlaying = false;
	}

	bool canDelete() {
		int state = 0;
		alGetSourcei(source, AL_SOURCE_STATE, &state);

		return state == AL_STOPPED;
	}
};