#pragma once
#include "RayTracer.h"

class Renderer final : public MemoryOverride<Renderer>
{
public:
	static Heap* _heap;

public:
	Renderer(const unsigned &w = 640, const unsigned &h = 480);
	~Renderer();
	
	void BasicRender();
	void SimpleShrinking();
	void SmoothScalingOptimized();
	void OutputSpeedData(const double& endCount, const int& numFrames);
	void SmoothScalingUO();

private:
	Vec3f *image;

	unsigned width;
	unsigned height;

	float aspectratio;
	float invHeight;
	float invWidth;
	float fov;
	float angle;

private:
	void SaveToFile(const int& iteration) const;
	void Render(const std::vector<Sphere*> &spheres, const int &iteration);
};

