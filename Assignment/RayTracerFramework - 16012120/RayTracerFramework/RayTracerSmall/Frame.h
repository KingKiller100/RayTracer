#pragma once
#include "Maths/Vec3.h"

typedef maths::Vec3<double> Vec3d;

struct Frame
{
	char objectID;
	int keyFrame;
	Vec3d pos;
	Vec3d scale;
	Vec3d brushColour;
	Vec3d colour;
};