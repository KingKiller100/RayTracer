#pragma once
#include "RayTracer.h"
#include <mutex>

class Renderer final : public MemoryOverride<Renderer>
{
public:
	static Heap* _heap;

public:
	Renderer(const unsigned &w = 640, const unsigned &h = 480);
	~Renderer();
	
	// void BasicRender();
	void SimpleShrinking();
	void SmoothScalingOptimized();
	void OutputSpeedData(const double& endCount, const int& numFrames);
	void SmoothScalingUO();

private:

	Vec3f *mainImage;

	Vec3c *trueImage;

	unsigned width;
	unsigned height;
	unsigned numOfCores;

	float aspectRatio;
	float invHeight;
	float invWidth;
	float fov;
	float angle;

private:
	void SaveToFile(const int& iteration) const;
	void Render(const std::vector<Sphere*> &spheres, const int &iteration);
	void RayTraceThreaded(const std::vector<Sphere*>& spheres, Vec3f* imageSegment, Vec3c* trueImageSegment);
	void RayTrace(const std::vector<Sphere*>& spheres, Vec3f *imageSegment, Vec3c *trueImageSegment);
	void SaveSSFile(std::stringstream * ss, const int &iteration);
	void SaveToStringStreamThreaded(std::stringstream &ss, const int &threadIndex);
};

