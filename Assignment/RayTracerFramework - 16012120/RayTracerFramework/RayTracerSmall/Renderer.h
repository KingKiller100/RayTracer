#pragma once
#include "RayTracer.h"
#include "Animation/FramesCollection.h"

class Renderer : public MemoryOverride<Renderer>
{
public:
	static Heap* _heap;

public:
	Renderer(const unsigned &w = 640, const unsigned &h = 480);
	~Renderer();


	void BasicRender();
	void SimpleShrinking();
	void SmoothScalingOptimized();
	void SmoothScalingUO();

private:
	// RayTracer *rayTracer;

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

