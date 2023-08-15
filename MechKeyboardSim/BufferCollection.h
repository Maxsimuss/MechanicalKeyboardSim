#pragma once
#include <vector>
#include "AudioBuffer.h"

class BufferCollection {
private:
	std::vector<AudioBuffer*> buffers;
	int lastPlayed = 0;

public:
	~BufferCollection() {
		for (auto buffer : buffers) {
			delete buffer;
		}
	}

	void addBuffer(AudioBuffer* buffer) {
		buffers.push_back(buffer);
	}

	AudioBuffer* getBuffer() {
		if (buffers.size() < 1) {
			throw buffers.size();
		}

		if (buffers.size() == 1) {
			return buffers[0];
		}

		int index = rand() % buffers.size();
		if (index == lastPlayed) {
			index++;
			index %= buffers.size();
		}

		return buffers[index];
	}
};

