#pragma once
#include "PlaybackStartType.h"

struct AudioSourceConfig {
public:
	bool relative = false;
	bool loop = false;

	PlaybackStartType playbackStartType = PlaybackStartType::Predefined;
	double playbackStartTime = 0;
};