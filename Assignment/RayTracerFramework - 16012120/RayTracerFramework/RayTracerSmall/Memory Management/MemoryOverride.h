#pragma once 
#include <typeinfo>
#include "HeapFactory.h"
#include "MemoryStructures.h"

template<class X>
class MemoryOverride
{	
public:
	void * operator new(size_t size)
	{
		if (!X::_heap) // For each class created, make a static heap object "_heap"
			X::_heap = HeapFactory::CreateHeap(typeid(X).name());

		return ::operator new(size, X::_heap);
	}

	void * operator new[](size_t size)
	{
		return MemoryOverride::operator new(size);
	}

	void operator delete(void *pMem, size_t size)
	{
		::operator delete(pMem);
	}

	void operator delete[](void *pMem, size_t size)
	{
		::operator delete[](pMem);
	}
};

