#pragma once
#include "Shapes/Sphere.h"

class RayTracer : public MemoryOverride<RayTracer>
{
public:
	static Heap* _heap;

public:
	static RayTracer *GetInstance();

	~RayTracer();

	static Vec3f Trace(const Vec3f &raydir, const std::vector<Sphere> &spheres, const int &depth = 0, const Vec3f &rayorig = Vec3f(0));

private:
	static RayTracer *instance;
	static Sphere currentSphere;

private:
	RayTracer();
	static float Mix(const float& a, const float& b, const float& mix);

};

