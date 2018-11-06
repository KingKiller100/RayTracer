#pragma once
#include "../Memory Management/MemoryOverride.h"
#include "../Maths/Vec3.h"

typedef maths::Vec3<float> Vec3f;

class Sphere : MemoryOverride<Sphere>
{
public:
	static Heap *heap;
	Vec3f center;                           /// position of the sphere
	float radius, radius2;                  /// sphere radius and radius^2
	Vec3f surfaceColor, emissionColor;      /// surface color and emission (light)
	float transparency, reflection;         /// surface transparency and reflectivity
	
public:
	Sphere(
		const Vec3f &c,
		const float &r,
		const Vec3f &sc,
		const float &refl,
		const float &transp = 0,
		const Vec3f &ec = 0);
		
	// Compute a ray-sphere intersection using the geometric solution
	bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1) const;
};
