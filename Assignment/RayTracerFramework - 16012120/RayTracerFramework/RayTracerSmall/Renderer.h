#pragma once
#include "Shapes/Sphere.h"
#include "RayTracer.h"
#include "framesCollection.h"

class Renderer : public MemoryOverride<Renderer>
{
public:
	static Heap* _heap;

	Renderer(const unsigned &w = 640, const unsigned &h = 480);
	~Renderer();

	void Render(const std::vector<Sphere> &spheres, const int &iteration);

	void BasicRender();
	void SimpleShrinking();
	void SmoothScaling();

private:
	unsigned width;
	unsigned height;
	Vec3f *image;

	Animation animations;
};

