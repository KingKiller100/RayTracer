#include "FrameList.h"

Heap *FramesList::_heap = nullptr;

FramesList::FramesList(int size)
{
	for (int i = 0; i < size; i++)
		framesList.emplace_back(new Frame());
}

FramesList::~FramesList()
{
	for (auto* frame : framesList)
	{
		delete frame;
		frame = nullptr;
	}

	framesList.clear();
}

auto FramesList::Count() const -> int
{
	return framesList.size();
}

void FramesList::resize(unsigned size)
{
	framesList.resize(size);
}

void FramesList::Add(Frame* frame)
{
	framesList.emplace_back(frame);
}