#pragma once
#include "Shapes/Sphere.h"

class RayTracer : public MemoryOverride<RayTracer>
{
public:
	static Heap* _heap;

public:
	static RayTracer *GetInstance();

	~RayTracer();

	static float Mix(const float& a, const float& b, const float& mix);
	static Vec3f Trace(const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, const int &depth);

private:
	static RayTracer *instance;
	static Sphere* sphere;

private:
	RayTracer();

};

