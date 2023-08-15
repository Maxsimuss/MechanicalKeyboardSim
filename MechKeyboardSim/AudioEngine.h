#pragma once
#include <AL/alc.h>
#include <map>
#include <list>
#include <string>
#include "BufferCollection.h"
#include "SoundSource.h"
#include "AudioSourceConfig.h"

class AudioEngine {
private:
	std::map<std::string, BufferCollection*> buffers;
	std::list<SoundSource*> soundSources;

	ALCdevice* device;
	ALCcontext* context;
	int maxSources = 0;

	float volume = 1;

public:
	AudioEngine() {
		device = alcOpenDevice(nullptr);
		context = alcCreateContext(device, nullptr);

		alcMakeContextCurrent(context);
		alcGetIntegerv(device, ALC_MONO_SOURCES, 1, &maxSources);
	}

	~AudioEngine() {
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void setVolume(float volume) {
		this->volume = volume;

		alListenerf(AL_GAIN, volume);
	}

	float getVolume() {
		return volume;
	}

	void registerSound(std::string name, AudioSourceConfig sourceConfig, std::string path);
	void registerSound(std::string name, std::string path) {
		registerSound(name, AudioSourceConfig(), path);
	}

	void unregisterSound(std::string name);
	void playSound(std::string name);
};