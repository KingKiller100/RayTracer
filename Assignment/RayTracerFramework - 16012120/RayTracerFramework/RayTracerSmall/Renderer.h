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
	void SampleFrames();
	void SmoothScalingOptimized(const bool &isThreaded);
	void SmoothScalingUO();

private:

	Vec3f *mainImage;
	
	unsigned width;
	unsigned height;
	unsigned numOfCores;

	float aspectRatio;
	float invHeight;
	float invWidth;
	float fov;
	float angle;

private:
	void Render(const std::vector<Sphere*> &spheres, const int &iteration);
	void RenderThreaded(const std::vector<Sphere*>& spheres, const int& iteration);
	void RayTraceThreaded(const std::vector<Sphere*>& spheres, Vec3f* imageSegment, const int &threadIndex, std::string *s);
	void RayTrace(const std::vector<Sphere*>& spheres, Vec3f *imageSegment, std::string &fileInfo);
	void SaveSSFile(std::string** s, const int &iteration);
	void SaveToFile(const int& iteration, std::string &fileInfo) const;
	void OutputSpeedData(const double& endCount, const int& numFrames);
	void OutputSpeedDataThreaded(const double& endCount, const int& numFrames);
};

