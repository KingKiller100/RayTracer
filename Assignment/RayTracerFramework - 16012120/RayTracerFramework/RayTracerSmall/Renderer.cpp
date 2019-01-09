#pragma once

#include "Renderer.h"
#include "Animation/FramesCollection.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>

#define M_PI 3.141592653589793

Heap* Renderer::_heap = nullptr;

typedef std::chrono::milliseconds Milliseconds;

Renderer::Renderer(const unsigned &w, const unsigned &h): width(w), height(h), image(nullptr), fov(30.f)
{
	aspectratio = float(width / float(height));
	invWidth = 1 / float(width);
	invHeight = 1 / float(height);
	angle = tan(M_PI * 0.5 * fov / 180.);

	// rayTracer = RayTracer::GetInstance();
}


Renderer::~Renderer()
{
	delete image;
	image = nullptr;
}

//[comment]
// Main rendering function. We compute a camera ray for each pixel of the image
// trace it and return a color. If the ray hits a sphere, we return the color of the
// sphere at the intersection point, else we return the background color.
//[/comment]
void Renderer::Render(const std::vector<Sphere*>& spheres, const int &iteration)
{
 	image = new Vec3f[width * height];
	Vec3f *pixel = image;
	float xx;
	float yy;
	// Trace rays
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x, ++pixel) {
			xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
			yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			Vec3f raydir(xx, yy, -1);
			raydir.normalize();
			*pixel = RayTracer::Trace(raydir, spheres);
		}
	}

	SaveToFile(iteration);
	
	delete[] image;
}

void Renderer::SaveToFile(const int &iteration) const
{
	// Save result to a PPM image (keep these flags if you compile under Windows)
	std::stringstream fileInfo;
	std::string tempString = std::string("./spheres") + std::to_string(iteration) +  std::string(".ppm");
	char* filename = (char*)tempString.c_str();

	std::ofstream ofs(filename, std::ios::out | std::ios::binary);
	for (unsigned i = 0; i < width * height; ++i) {
		fileInfo << 
			(unsigned char)(std::min(float(1), image[i].x) * 255) <<
			(unsigned char)(std::min(float(1), image[i].y) * 255) <<
			(unsigned char)(std::min(float(1), image[i].z) * 255);
	}

	ofs << "P6\n" << width << " " << height << "\n255\n";
	ofs.write(fileInfo.str().c_str(), height * width * 3);
	ofs.close();

	std::cout << "Rendered and saved " << filename << std::endl;
}

void Renderer::BasicRender()
{
	std::vector<Sphere*> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	spheres.push_back(new Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
	spheres.push_back(new Sphere(Vec3f(0.0, 0, -20), 4, Vec3f(1.00, 0.32, 0.36), 1, 0.5)); // The radius paramter is the value we will change
	spheres.push_back(new Sphere(Vec3f(5.0, -1, -15), 2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
	spheres.push_back(new Sphere(Vec3f(5.0, 0, -25), 3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));
	
	// This creates a file, titled 1.ppm in the current working directory
	Render(spheres, 1);
}

void Renderer::SimpleShrinking()
{
	std::vector<Sphere*> spheres;
	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)

	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			spheres.push_back(new Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
			spheres.push_back(new Sphere(Vec3f(0.0, 0, -20), 4, Vec3f(1.00, 0.32, 0.36), 1, 0.5)); // The radius paramter is the value we will change
			spheres.push_back(new Sphere(Vec3f(5.0, -1, -15), 2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
			spheres.push_back(new Sphere(Vec3f(5.0, 0, -25), 3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));

		}
		else if (i == 1)
		{
			spheres.push_back(new Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
			spheres.push_back(new Sphere(Vec3f(0.0, 0, -20), 3, Vec3f(1.00, 0.32, 0.36), 1, 0.5)); // Radius--
			spheres.push_back(new Sphere(Vec3f(5.0, -1, -15), 2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
			spheres.push_back(new Sphere(Vec3f(5.0, 0, -25), 3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));
		}
		else if (i == 2)
		{
			spheres.push_back(new Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
			spheres.push_back(new Sphere(Vec3f(0.0, 0, -20), 2, Vec3f(1.00, 0.32, 0.36), 1, 0.5)); // Radius--
			spheres.push_back(new Sphere(Vec3f(5.0, -1, -15), 2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
			spheres.push_back(new Sphere(Vec3f(5.0, 0, -25), 3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));
		}
		else if (i == 3)
		{
			spheres.push_back(new Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
			spheres.push_back(new Sphere(Vec3f(0.0, 0, -20), 1, Vec3f(1.00, 0.32, 0.36), 1, 0.5)); // Radius--
			spheres.push_back(new Sphere(Vec3f(5.0, -1, -15), 2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
			spheres.push_back(new Sphere(Vec3f(5.0, 0, -25), 3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));
		}

		Render(spheres, i);

		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}
}

void Renderer::SmoothScalingOptimized()
{
	auto startRender = std::chrono::system_clock::now();

	FramesCollection animations; 

	std::vector<Sphere*> spheres;

	int numOfFrames = 101;

	animations.ReadJSON("Data\\spheresData.json", numOfFrames);

	numOfFrames--;

	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)
	for (float r = 0; r <= numOfFrames; ++r)
	{
		auto start = std::chrono::system_clock::now();

		for (int i = 0; i < animations.numOfObjects; ++i)
		{
			Frame frame = *animations.framesCollection[i]->framesList[r];
			animations.UpdateFrame(frame, r, numOfFrames);
			spheres.emplace_back(new Sphere(frame.pos, frame.scale, frame.brushColour, 1, 0.0));
		}

		Render(spheres, r);

		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();

		auto end = std::chrono::duration_cast <Milliseconds>(std::chrono::system_clock::now() - start);
		
		std::cout << "Render Time: " << end.count() << "ms" << std::endl;
	}

	auto endRender = std::chrono::duration_cast <Milliseconds>(std::chrono::system_clock::now() - startRender);

	std::cout << "Final Render Time: " << endRender.count() << "ms" << std::endl;
	std::cout << "Average Time: " << endRender.count() / numOfFrames << std::endl;
}

void Renderer::SmoothScalingUO()
{
	std::vector<Sphere*> spheres;

	// Vector structure for Sphere (position, radius, surface color, reflectivity, transparency, emission color)
	for (float r = 0; r <= 100; r++)
	{
		spheres.push_back(new Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
		spheres.push_back(new Sphere(Vec3f(0.0, 0, -20), r / 100, Vec3f(1.00, 0.32, 0.36), 0, 0.5)); // Radius++ change here
		spheres.push_back(new Sphere(Vec3f(5.0, -1, -15), 2, Vec3f(0.90, 0.76, 0.46), 0, 0.0));
		spheres.push_back(new Sphere(Vec3f(5.0, 0, -25), 3, Vec3f(0.65, 0.77, 0.97), 0, 0.0));
		Render(spheres, r);
		std::cout << "Rendered and saved spheres" << r << ".ppm" << std::endl;

		// Dont forget to clear the Vector holding the spheres.
		spheres.clear();
	}
} // Original RayTracer Code
