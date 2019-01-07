#pragma once
#include "Heap.h"
#include <vector>

class HeapFactory final
{
public:
	HeapFactory();
	~HeapFactory();

	static Heap *CreateHeap(const char * szName);
	static Heap* GetDefaultHeap();

	static void WalkTheHeap(const size_t &id);

private:
	static Heap *_defaultHeap;
	static std::vector<Heap*> heapContainer;
};
