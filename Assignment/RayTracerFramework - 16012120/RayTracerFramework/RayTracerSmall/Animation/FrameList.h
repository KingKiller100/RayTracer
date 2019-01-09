#pragma once
#include "Frame.h"
#include <vector>

class FramesList : public MemoryOverride<FramesList>
{
public:
	static Heap *_heap;
	std::vector<Frame*> framesList;

	FramesList(int size);

	~FramesList();

	int Count() const;

	void resize(unsigned size);

	void Add(Frame* frame);
};