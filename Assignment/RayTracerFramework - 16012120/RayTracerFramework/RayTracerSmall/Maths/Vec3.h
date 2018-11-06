#pragma once
#include <iostream>
#include <cmath>
#include "../Memory Management/Heap.h"

namespace  maths
{
	template<typename T>
	struct Vec3
	{
		static Heap *v3_pHeap;
		
		T x, y, z;
		Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
		Vec3(T xx) : x(xx), y(xx), z(xx) {}
		Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
		
		Vec3& normalize()
		{
			T nor2 = length2();
			if (nor2 > 0)
			{
				T invNor = 1 / sqrt(nor2);
				x *= invNor, y *= invNor, z *= invNor;
			}

			return *this;
		}

		T dot(const Vec3<T> &v) const														{ return x * v.x + y * v.y + z * v.z; }
		T length2() const																	{ return x * x + y * y + z * z; }
		T length() const																	{ return sqrt(length2()); }


		// Mathematical instruments overloads
		Vec3<T> operator * (const T &f) const												{ return Vec3<T>(x * f, y * f, z * f); }
		Vec3<T> operator * (const Vec3<T> &v) const											{ return Vec3<T>(x * v.x, y * v.y, z * v.z); }
		Vec3<T> operator - (const Vec3<T> &v) const											{ return Vec3<T>(x - v.x, y - v.y, z - v.z); }
		Vec3<T> operator + (const Vec3<T> &v) const											{ return Vec3<T>(x + v.x, y + v.y, z + v.z); }
		Vec3<T>& operator += (const Vec3<T> &v)												{ return Vec3<T>(x += v.x, y += v.y, z += v.z); }
		Vec3<T>& operator *= (const Vec3<T> &v)												{ return Vec3<T>(x *= v.x, y *= v.y, z *= v.z); }
		Vec3<T> Reverse() const																{ return Vec3<T>(-x, -y, -z); }
		
		friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
		{
			os << "[" << v.x << " " << v.y << " " << v.z << "]";
			return os;
		}

		void *operator new(size_t size);
		void operator delete(void *pMem, size_t size);
	};
}