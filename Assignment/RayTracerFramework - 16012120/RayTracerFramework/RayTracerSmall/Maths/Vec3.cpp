#include "Vec3.h"
#include "../Memory Management/HeapFactory.h"
#include "../Memory Management/MemoryStructures.h"

namespace maths
{
	template <typename T>
	Heap *Vec3<T>::v3_pHeap = nullptr;
	
	template <typename T>
	void* Vec3<T>::operator new(size_t size)
	{
		if (!v3_pHeap)
			v3_pHeap = HeapFactory::CreateHeap("Vector3");

		return MemoryManagement::operator new(sizeof(Vec3));
	}

	template <typename T>
	void Vec3<T>::operator delete(void* pMem, size_t size)
	{
		MemoryManagement::operator delete(pMem);
	}
}


