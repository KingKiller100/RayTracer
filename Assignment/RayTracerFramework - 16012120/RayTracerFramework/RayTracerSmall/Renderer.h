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
	void SmoothScaling();
	void SmoothScalingUO();

private:
	unsigned width;
	unsigned height;
	Vec3f *image;
	float  aspectratio;

	float invHeight;
	float invWidth;
	float fov;
	float angle;

private:
	void SaveToFile(const int& iteration);
	void Render(const std::vector<Sphere> &spheres, const int &iteration);
};

