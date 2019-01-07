#include "Vec3.h"
#include "../Memory Management/HeapFactory.h"
#include "../Memory Management/MemoryStructures.h"

namespace maths
{
	template <typename T>
	Heap *Vec3<T>::_heap = nullptr;
}


