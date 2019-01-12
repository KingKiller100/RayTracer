#include "Renderer.h"
#include "Animation/framesCollection.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <complex>

#define M_PI 3.141592653589793

Heap* Renderer::_heap = nullptr;

typedef std::chrono::milliseconds Milliseconds;
typedef std::thread Thread;

Renderer::Renderer(const unsigned &w, const unsigned &h): width(w), height(h), mainImage(nullptr), fov(30.f), numOfCores(0)
{
	aspectRatio = float(width / float(height));
	invWidth = 1 / float(width);
	invHeight = 1 / float(height);
	angle = tan(M_PI * 0.5 * fov / 180.);

 	mainImage = new Vec3f[width * height];
	trueImage = new Vec3c[width * height];
}


Renderer::~Renderer()
{
	delete[] mainImage;
	mainImage = nullptr;

	delete[] trueImage;
	trueImage = nullptr;
}

//[comment]
// Main rendering function. We compute a camera ray for each pixel of the image
// trace it and return a color. If the ray hits a sphere, we return the color of the
// sphere at the intersection point, else we return the background color.
//[/comment]
void Renderer::Render(const std::vector<Sphere*>& spheres, const int &iteration)
{
	// mutex.lock();
	numOfCores = std::thread::hardware_concurrency();
	auto** threads = new Thread*[numOfCores];

	Vec3f *im = mainImage;
	Vec3c *ti = trueImage;

	for (unsigned i = 0; i < numOfCores; ++i)	
		threads[i] = new Thread(&Renderer::RayTraceThreaded, this, spheres, mainImage + (width * ((height / numOfCores) * i)), trueImage + (width * ((height / numOfCores) * i)));
		
	for (unsigned i = 0; i < numOfCores; ++i)
		threads[i]->join();

	// RayTrace(spheres, im, ti);

	// auto* ss = new std::stringstream[numOfCores];
	//
	// for (unsigned i = 0; i < numOfCores; ++i)	
	// 	threads[i] = new Thread(&Renderer::SaveToStringStreamThreaded, this, ss[i], i);
	//
	//
	// for (unsigned i = 0; i < numOfCores; ++i)
	// 	threads[i]->join();
	//
	// SaveSSFile(ss, iteration);

	SaveToFile(iteration);
	
	// mutex.unlock();
}

void Renderer::RayTraceThreaded(const std::vector<Sphere*>& spheres, Vec3f *imageSegment, Vec3c *trueImageSegment)
{
	float xOffset;
	float yOffset;
	unsigned limit = height / numOfCores;

	// Trace rays
	for (unsigned y = 0; y < limit; ++y) {
		for (unsigned x = 0; x < width; ++x, ++imageSegment, ++trueImageSegment) {
			xOffset = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio;
			yOffset = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			Vec3f raydir(xOffset, yOffset, -1);
			raydir.normalize();

			*imageSegment = RayTracer::GetInstance()->Trace(raydir, spheres);
			*trueImageSegment = Vec3c((unsigned char)(std::min(1.0f, imageSegment->x) * 255), (unsigned char)(std::min(1.0f, imageSegment->y) * 255), (unsigned char)(std::min(1.0f, imageSegment->z) * 255));
			
			if (trueImageSegment->x != 255)
				int a = 10;
		}
	}
}

void Renderer::RayTrace(const std::vector<Sphere*>& spheres, Vec3f *imageSegment, Vec3c *trueImageSegment)
{
	float xOffset;
	float yOffset;

	// Trace rays
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x, ++imageSegment, ++trueImageSegment) {
			xOffset = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio;
			yOffset = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			Vec3f raydir(xOffset, yOffset, -1);
			raydir.normalize();
			*imageSegment = RayTracer::GetInstance()->Trace(raydir, spheres);
			*trueImageSegment = Vec3c(static_cast<unsigned char>(std::min(float(1), imageSegment->x) * 255),
				static_cast<unsigned char>(std::min(float(1), imageSegment->y) * 255),
				static_cast<unsigned char>(std::min(float(1), imageSegment->z) * 255));

			if (trueImageSegment->x != 255)
			int a = 10;
		}
	}
}


void Renderer::SaveSSFile(std::stringstream *ss, const int &iteration)
{
	std::string tempString = std::string("./spheres") + std::to_string(iteration) + std::string(".ppm");
	char* filename = const_cast<char*>(tempString.c_str());

	std::stringstream fileInfo;
	for (unsigned i = 0; i < numOfCores; ++i)
		fileInfo << ss[i].str();

	std::ofstream ofs(filename, std::ios::out | std::ios::binary);

	ofs.flush();
	ofs << "P6\n" << width << " " << height << "\n255\n";
	ofs.write(fileInfo.str().c_str(), height * width * 3);
	ofs.close();

	std::cout << "Rendered and saved " << filename << std::endl;
}

void Renderer::SaveToStringStreamThreaded(std::stringstream &ss, const int &threadIndex) 
{
	const unsigned difference = (height / numOfCores);
	const unsigned startPoint =  difference * threadIndex;

	for (unsigned i = startPoint; i < startPoint + difference; ++i)
	{
		ss << trueImage[i].x << trueImage[i].y << trueImage[i].z;
	}
}

void Renderer::SaveToFile(const int &iteration) const
{
	// Save result to a PPM image (keep these flags if you compile under Windows)
	std::stringstream fileInfo;
	std::string tempString = std::string("./spheres") + std::to_string(iteration) + std::string(".ppm");
	char* filename = (char*)tempString.c_str();

	std::ofstream ofs(filename, std::ios::out | std::ios::binary);

	for (unsigned i = 0; i < width * height; ++i) {
		fileInfo << trueImage[i].x << trueImage[i].y << trueImage[i].z;
	}

	ofs.flush();
	ofs << "P6\n" << width << " " << height << "\n255\n";
	// ofs << fileInfo.str().c_str();
	ofs.write(fileInfo.str().c_str(), height * width * 3);
	ofs.close();

	std::cout << "Rendered and saved " << filename << std::endl;
}

// void Renderer::BasicRender()
// {
// 	int numOfFrames = 1;
// 	FramesCollection animations;
// 	animations.ReadJSON("Data\\spheresData.json", numOfFrames);
//
// 	std::vector<Sphere*> spheres;
//
// 	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)
// 	for (unsigned i = 0; i < animations.numOfObjects; ++i)
// 	{
// 		Frame frame = *animations.framesCollection[i]->framesList[0];
// 		animations.UpdateFrame(frame, i, numOfFrames);
// 		spheres.emplace_back(new Sphere(frame.pos, frame.scale, frame.brushColour, 1, 0.0));
// 	}
// 		
// 	// This creates a file, titled 1.ppm in the current working directory
// 	Render(spheres, 1);
//
// 	spheres.clear();
// }
//
void Renderer::SimpleShrinking()
{
	auto startRender = std::chrono::system_clock::now();
	
	int numOfFrames = 4;

	FramesCollection animations;
	animations.ReadJSON("Data\\spheresData.json", 101);

	std::vector<Sphere*> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	for (int i = 0; i < numOfFrames; i++)
	{
		auto start = std::chrono::system_clock::now();

		Frame frame1 = *animations.framesCollection[0]->framesList[i];
		Frame frame2 = *animations.framesCollection[1]->framesList[i];
		Frame frame3 = *animations.framesCollection[2]->framesList[i];
		Frame frame4 = *animations.framesCollection[3]->framesList[i];

		animations.UpdateFrame(frame1, 0, 4);
		animations.UpdateFrame(frame2, 1, 4);
		animations.UpdateFrame(frame3, 2, 4);
		animations.UpdateFrame(frame4, 3, 4);

		if (i == 0)
		{
			spheres.emplace_back(new Sphere(frame1.pos, frame1.scale, frame1.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame2.pos, frame2.scale, frame2.brushColour, 1, 0.25));
			spheres.emplace_back(new Sphere(frame3.pos, frame3.scale, frame3.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame4.pos, frame4.scale, frame4.brushColour, 1, 0.0));

		}
		else if (i == 1)
		{
			spheres.emplace_back(new Sphere(frame1.pos, frame1.scale, frame1.brushColour, 1, 0.7));
			spheres.emplace_back(new Sphere(frame2.pos, frame2.scale, frame2.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame3.pos, frame3.scale, frame3.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame4.pos, frame4.scale, frame4.brushColour, 1, 0.0));
		}
		else if (i == 2)
		{
			spheres.emplace_back(new Sphere(frame1.pos, frame1.scale, frame1.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame2.pos, frame2.scale, frame2.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame3.pos, frame3.scale, frame3.brushColour, 1, 0.75));
			spheres.emplace_back(new Sphere(frame4.pos, frame4.scale, frame4.brushColour, 1, 0.0));
		}
		else if (i == 3)
		{
			spheres.emplace_back(new Sphere(frame1.pos, frame1.scale, frame1.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame2.pos, frame2.scale, frame2.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame3.pos, frame3.scale, frame3.brushColour, 1, 0.0));
			spheres.emplace_back(new Sphere(frame4.pos, frame4.scale, frame4.brushColour, 1, 0.9));
		}

		Render(spheres, i);

		auto end = std::chrono::duration_cast <Milliseconds>(std::chrono::system_clock::now() - start);
		std::cout << "Render Time: " << end.count() << "ms" << std::endl;

		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}
	
	auto endRender = std::chrono::duration_cast <Milliseconds>(std::chrono::system_clock::now() - startRender);
	std::cout << "Final Render Time: " << endRender.count() << "ms" << std::endl;
	std::cout << "Average Time: " << endRender.count() / double(numOfFrames*1000) << " seconds" << std::endl;
}

void Renderer::SmoothScalingOptimized()
{
	auto startRender = std::chrono::system_clock::now();

	FramesCollection animations;
	int numOfFrames = 101;
	std::vector<Sphere*> spheres;

	animations.ReadJSON("Data\\spheresData.json", numOfFrames);
	
	numOfFrames--;

	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)
	for (unsigned r = 0; r <= numOfFrames; ++r)
	{
		auto start = std::chrono::system_clock::now();

		for (unsigned i = 0; i < animations.numOfObjects; ++i)
		{
			Frame frame = *animations.framesCollection[i]->framesList[r];
			animations.UpdateFrame(frame, r, numOfFrames);
			spheres.emplace_back(new Sphere(frame.pos, frame.scale, frame.brushColour, 1, 0.0));
		}

		Render(spheres, int(r));

		auto end = std::chrono::duration_cast <Milliseconds>(std::chrono::system_clock::now() - start);

		std::cout << "Render Time: " << end.count() << "ms" << std::endl;
		
		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}

	auto endRender = std::chrono::duration_cast <Milliseconds>(std::chrono::system_clock::now() - startRender);

	numOfFrames++;

	std::cout << "Final Render Time: " << endRender.count() << "ms" << std::endl;
	std::cout << "Average Time: " << endRender.count() / numOfFrames << std::endl;

	OutputSpeedData(endRender.count(), numOfFrames);
}

void Renderer::OutputSpeedData(const double &endCount, const int &numFrames)
{
	// Save result to a PPM image (keep these flags if you compile under Windows)
	std::string tempString = "./RenderSpeedData.txt";
	char* filename = (char*)tempString.c_str();

	std::ofstream ofs(filename, std::ios::out | std::ios::binary);

	ofs << "Final Render Time: " << endCount << "ms\r\n" <<
		"Average Time (milliseconds): " << endCount / numFrames << "ms\r\n" <<
		"Average Time (seconds) " << endCount / (numFrames * 1000) << "s";

	ofs.close();
}

void Renderer::SmoothScalingUO() // Original RayTracer Code
{
	auto numOfCores = std::thread::hardware_concurrency();
	auto** threads = new Thread*[numOfCores];

	std::vector<Sphere*> spheres;

	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)
	for (float r = 0; r <= 100; r += numOfCores)
	{
		for (unsigned i = 0; i < numOfCores; ++i)
		{
			spheres.push_back(new Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
			spheres.push_back(new Sphere(Vec3f(0.0, 0, -20), (r + i) / 100, Vec3f(1.00, 0.32, 0.36), 0, 0.5)); // Radius++ change here
			spheres.push_back(new Sphere(Vec3f(5.0, -1, -15), 2, Vec3f(0.90, 0.76, 0.46), 0, 0.0));
			spheres.push_back(new Sphere(Vec3f(5.0, 0, -25), 3, Vec3f(0.65, 0.77, 0.97), 0, 0.0));

			threads[i] = new Thread(&Renderer::Render, this, spheres, int(r+i));
		}

		for (unsigned i = 0; i < numOfCores; ++i)
		{
			threads[i]->join();
		}

		// std::cout << "Rendered and saved spheres" << r << ".ppm" << std::endl;

		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}
}
