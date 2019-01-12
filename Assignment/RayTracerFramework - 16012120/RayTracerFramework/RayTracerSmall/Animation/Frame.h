#pragma once
#include "../Memory Management/MemoryOverride.h"
#include "../Maths/Vec3.h"

struct Frame : MemoryOverride<Frame>
{
	static Heap *_heap;

	std::string objectID;
	int keyFrame;
	float scale;
	Vec3f pos;
	Vec3f brushColour;
	Vec3f colour;

	Frame();
};