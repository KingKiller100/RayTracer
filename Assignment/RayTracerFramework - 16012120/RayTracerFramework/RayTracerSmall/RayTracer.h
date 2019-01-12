#pragma once
#include "Shapes/Sphere.h"

class RayTracer : public MemoryOverride<RayTracer>
{
public:
	static Heap* _heap;

public:
	static RayTracer *GetInstance();

	~RayTracer();

	Vec3f Trace(const Vec3f &raydir, const std::vector<Sphere*> &spheres, const int &depth = 0, const Vec3f &rayorig = Vec3f(0)) const;

private:
	static RayTracer *instance;
	static  Sphere* sphere;

private:
	RayTracer();
	float Mix(const float& a, const float& b, const float& mix) const;
};

