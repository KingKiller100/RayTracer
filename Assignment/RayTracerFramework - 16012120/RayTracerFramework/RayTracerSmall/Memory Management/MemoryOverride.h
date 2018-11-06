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
		if (!X::heap)
			X::heap = HeapFactory::CreateHeap(typeid(X).name());

		return MemoryManagement::operator new(size, X::heap);
	}

	void operator delete(void *pMem, size_t size)
	{
		MemoryManagement::operator delete(pMem);
	}
};