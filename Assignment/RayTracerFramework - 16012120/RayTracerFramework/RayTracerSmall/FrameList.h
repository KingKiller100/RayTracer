#pragma once
#include "Frame.h"
#include <vector>

class FramesList
{
public:
	std::vector<Frame*> framesList;

	FramesList(int size)
	{
		for (int i = 0; i < size; i++)
			framesList.emplace_back(new Frame());
	}

	~FramesList()
	{
		for (Frame* frame : framesList)
		{
			delete frame;
			frame = nullptr;
		}

		framesList.clear();
	}

	int Count()
	{
		return framesList.size();
	}

	void resize(unsigned size)
	{
		framesList.resize(size);
	}

	void Add(Frame* frame)
	{
		framesList.emplace_back(frame);
	}
};
