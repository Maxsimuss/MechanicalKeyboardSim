#include "AudioEngine.h"
#include "AudioData.h"
#include "WaveFileReader.h"

void AudioEngine::registerSound(std::string name, AudioSourceConfig sourceConfig, std::string path) {
	WaveFileReader wave(path);

	AudioData data = wave.parseSamples();
	wave.cleanup();

	AudioBuffer* audioBuffer = new AudioBuffer(data, data.sampleRate, sourceConfig);
	delete[] data.data;

	if (!buffers.count(name)) {
		buffers[name] = new BufferCollection();
	}

	buffers[name]->addBuffer(audioBuffer);
}

void AudioEngine::unregisterSound(std::string name) {
	delete buffers[name];

	buffers.erase(name);
}

void AudioEngine::playSound(std::string name) {
	soundSources.remove_if([](SoundSource* source) {
		if (source->canDelete()) {
			source->stop();
			delete source;

			return true;
		}

		return false;
	});

	AudioBuffer* buffer = buffers[name]->getBuffer();

	SoundSource* soundSource = new SoundSource(buffer, 1.0f, 1.0f);
	soundSource->play();

	soundSources.push_back(soundSource);
}