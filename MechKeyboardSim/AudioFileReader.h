#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "AudioData.h"

class AudioFileReader {
public:
	AudioFileReader(std::string path) {
		std::ifstream file(path, std::ios::binary);

		file.seekg(0, std::ios::end);
		uint32_t size = file.tellg();
		data = new char[size];
		dataLength = size;

		file.seekg(0, std::ios::beg);
		file.read(data, size);
		file.close();
	}

	virtual AudioData parseSamples() = 0;

	void cleanup() {
		delete[] data;
	}
protected:
	char* data;
	uint32_t dataLength;
};

