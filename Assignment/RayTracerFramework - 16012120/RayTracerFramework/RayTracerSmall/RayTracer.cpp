#include "RayTracer.h"
#include <algorithm>

// This variable controls the maximum recursion depth
#define MAX_RAY_DEPTH 5

RayTracer *RayTracer::instance = nullptr;
Heap *RayTracer::_heap = nullptr;
Sphere RayTracer::currentSphere = NULL;

RayTracer* RayTracer::GetInstance()
{
	if (!instance)
		instance = new RayTracer;

	return instance;
}

RayTracer::RayTracer()
{
	// currentSphere = new Sphere();
}


RayTracer::~RayTracer() = default;

float RayTracer::Mix(const float &a, const float &b, const float &mix)
{
	return b * mix + a * (1 - mix);
}

//[comment]
// This is the main trace function. It takes a ray as argument (defined by its origin
// and direction). We test if this ray intersects any of the geometry in the scene.
// If the ray intersects an object, we compute the intersection point, the normal
// at the intersection point, and shade this point using this information.
// Shading depends on the surface property (is it transparent, reflective, diffuse).
// The function returns a color for the ray. If the ray intersects an object that
// is the color of the object at the intersection point, otherwise it returns
// the background color.
//[/comment]
Vec3f RayTracer::Trace(const Vec3f& raydir, const std::vector<Sphere>& spheres, const int& depth, const Vec3f& rayorig)
{
	int sphereListSize = spheres.size();

	//if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
	float tnear = INFINITY;
	float t0, t1;

	// find intersection of this ray with the sphere in the scene
	for (unsigned i = 0; i < sphereListSize; ++i) {
		t0 = t1 = INFINITY;
		if (spheres[i].intersect(rayorig, raydir, t0, t1)) {
			if (t0 < 0) t0 = t1;
			if (t0 < tnear) {
				tnear = t0;
				currentSphere = spheres[i];
			}
		}
	}

	// if there's no intersection return black or background color
	if (currentSphere.radius2 == NULL) return Vec3f(2);

	Vec3f surfaceColor = 0; // color of the ray/surfaceof the object intersected by the ray
	Vec3f phit = rayorig + raydir * tnear; // point of intersection
	Vec3f nhit = phit - currentSphere.center; // normal at the intersection point
	nhit.normalize(); // normalize normal direction
					  // If the normal and the view direction are not opposite to each other
					  // reverse the normal direction. That also means we are inside the sphere so set
					  // the inside bool to true. Finally reverse the sign of IdotN which we want
					  // positive.
	float bias = 1e-4; // add some bias to the point from which we will be tracing
	bool inside = false;

	auto raydirDot = raydir.dot(nhit);

	nhit = raydirDot > 0 ? nhit.Reverse() : nhit;
	inside = raydirDot > 0;

	if ((currentSphere.transparency > 0 || currentSphere.reflection > 0) && depth < MAX_RAY_DEPTH) {
		float facingratio = -raydir.dot(nhit);
		// change the mix value to tweak the effect
		const float fresneleffect = Mix(pow(1 - facingratio, 3), 1, 0.1);
		// compute reflection direction (not need to normalize because all vectors
		// are already normalized)
		Vec3f refldir = raydir - nhit * 2 * raydirDot;
		refldir.normalize();
		Vec3f reflection = Trace(refldir, spheres, depth + 1, phit + nhit * bias);
		Vec3f refraction = 0;
		// if the sphere is also transparent compute refraction ray (transmission)
		if (currentSphere.transparency) 
		{
			float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface?
			float cosi = -nhit.dot(raydir);
			float k = 1 - eta * eta * (1 - cosi * cosi);
			Vec3f refrdir = raydir * eta + nhit * (eta *  cosi - sqrt(k));
			refrdir.normalize();
			refraction = Trace(refrdir, spheres, depth + 1, phit - nhit * bias);
		}
		// the result is a mix of reflection and refraction (if the sphere is transparent)
		surfaceColor = (
			reflection * fresneleffect +
			refraction * (1 - fresneleffect) * currentSphere.transparency) * currentSphere.surfaceColor;
	}
	else {
		// it's a diffuse object, no need to raytrace any further
		for (unsigned i = 0; i < sphereListSize; ++i) {
			if (spheres[i].emissionColor.x > 0) {
				// this is a light
				Vec3f transmission = 1;
				Vec3f lightDirection = spheres[i].center - phit;
				lightDirection.normalize();
				for (unsigned j = 0; j < sphereListSize; ++j) {
					if (i != j) {
						float t0, t1;
						if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
							transmission = 0;
							break;
						}
					}
				}
				surfaceColor += currentSphere.surfaceColor * transmission *
					std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor;
			}
		}
	}

	return surfaceColor + currentSphere.emissionColor;
}
