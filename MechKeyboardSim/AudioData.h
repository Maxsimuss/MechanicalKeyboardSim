#pragma once
struct AudioData
{
public:
	int sampleRate;
	int channels;
	int sampleCount;
	float* data;

	AudioData(int channels, int sampleCount, int sampleRate, float* data) : channels(channels), sampleCount(sampleCount), sampleRate(sampleRate), data(data) {}
};

