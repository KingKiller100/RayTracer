#pragma once
#include "HeapFactory.h"
#include <cassert>

#define MEMSYSTEM_SIGNATURE 3735929054 
#define MEMSYSTEM_ENDMARKER 3735928559

struct AllocHeader
{
	int iSignature;
	Heap *pHeap;
	int iSize;
	AllocHeader *_next;
	AllocHeader *_prev;
};

class MemoryManagement
{
public:
	static void * operator new(size_t size, Heap * pHeap)
	{
		size_t iRequestedBytes = size + sizeof(AllocHeader) + sizeof(int);
		char *pMem = (char*)malloc(iRequestedBytes);
		AllocHeader *pHeader = (AllocHeader*)pMem;
		pHeader->iSignature = MEMSYSTEM_SIGNATURE;
		pHeader->pHeap = pHeap;
		pHeader->iSize = (int)size;

		auto *pStartMemBlock = pMem + sizeof(AllocHeader);
		int *pEndMarker = (int*)(pStartMemBlock + size);
		*pEndMarker = MEMSYSTEM_ENDMARKER;

		pHeap->AddAllocation(size);

		return pStartMemBlock;
	}

	static void * operator new(size_t size)
	{
		return operator new(size, HeapFactory::GetDefaultHeap());
	}

	static void operator delete(void * pMem)
	{
		auto *pHeader = (AllocHeader*)((char*)pMem - sizeof(AllocHeader));

		auto heap = pHeader->pHeap;
		auto size = pHeader->iSize;
		auto sig = pHeader->iSignature;

		assert(sig == MEMSYSTEM_SIGNATURE);

		auto *pEndMarker = (int*)(int*)((char*)pMem + size);

		assert(*pEndMarker == MEMSYSTEM_ENDMARKER);

		heap->RemoveAllocation(size);
		free(pHeader);
	}
};