#pragma once
#include <ostream>
#include <cmath>
#include "../Memory Management/MemoryOverride.h"

namespace  maths
{
	template<typename T>
	struct Vec3 : MemoryOverride<Vec3<T>>
	{
		static Heap *_heap;
		
		T x, y, z;
		Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
		Vec3(T xx) : x(xx), y(xx), z(xx) {}
		Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
		
		Vec3& normalize()
		{
			if (length2() > 0)
			{
				T invNor = 1 / sqrt((T)length2());
				return *this *= invNor;
			}
		}

		T dot(const Vec3<T> &v) const														{ return x * v.x + y * v.y + z * v.z; }
		T length2() const																	{ return x * x + y * y + z * z; }
		T length() const																	{ return sqrt(length2()); }
		Vec3<T> Reverse() const																{ return Vec3<T>(-x, -y, -z); }


		// Mathematical instruments overloads
		Vec3<T> operator * (const T &f) const												{ return Vec3<T>(x * f, y * f, z * f); }
		Vec3<T> operator * (const Vec3<T> &v) const											{ return Vec3<T>(x * v.x, y * v.y, z * v.z); }
		Vec3<T> operator - (const Vec3<T> &v) const											{ return Vec3<T>(x - v.x, y - v.y, z - v.z); }
		Vec3<T> operator + (const Vec3<T> &v) const											{ return Vec3<T>(x + v.x, y + v.y, z + v.z); }
		Vec3<T>& operator += (const Vec3<T> &v)												{ return Vec3<T>(x += v.x, y += v.y, z += v.z); }
		Vec3<T>& operator *= (const Vec3<T> &v)												{ return Vec3<T>(x *= v.x, y *= v.y, z *= v.z); }
		// Vec3<T>& operator = (const Vec3<T> &v)
		// {
		// 	x = v.x;
		// 	y = v.y;
		// 	z = v.z; 
		// 	return *this;
		// }
		
		friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
		{
			os << "[" << v.x << " " << v.y << " " << v.z << "]";
			return os;
		}
	};
}

template <typename T>
Heap *maths::Vec3<T>::_heap = nullptr;

typedef maths::Vec3<float> Vec3f;
typedef maths::Vec3<unsigned char> Vec3c;
