#pragma once
#include "Maths/Vec3.h"

struct Frame
{
	std::string objectID;
	int keyFrame;
	float scale;
	Vec3f pos;
	Vec3f brushColour;
	Vec3f colour;

	Frame() : objectID(" "), keyFrame(-1), scale(1.f), pos(0), brushColour(255), colour(255)
	{ }
};