#pragma once
#include "AudioFileReader.h"

class WaveFileReader :
	public AudioFileReader {
public:
	WaveFileReader(std::string path) : AudioFileReader(path) {
	}

	virtual AudioData parseSamples();
};

