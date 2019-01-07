#pragma once
#include "Frame.h"
#include <vector>

class FramesList
{
public:
	std::vector<Frame*> framesList;

	FramesList()
	{
	}

	int Count()
	{
		return framesList.size();
	}

	void Add(Frame frame)
	{
		framesList.emplace_back(frame);
	}
};
