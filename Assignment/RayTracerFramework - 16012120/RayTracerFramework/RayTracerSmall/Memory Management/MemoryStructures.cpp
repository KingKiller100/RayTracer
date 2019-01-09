#include "MemoryStructures.h"
#include <iostream>
#include <cassert>

void * operator new(size_t size, Heap * pHeap)
{
	// memPool.Initialize();
	const size_t iRequestedBytes = size + sizeof(AllocHeader) + sizeof(int);
	//auto *pMem = static_cast<char*>(memPool.Alloc(iRequestedBytes));
	auto pMem = static_cast<char*>(malloc(iRequestedBytes));
	auto *pHeader = reinterpret_cast<AllocHeader*>(pMem);
	
	pHeader->iSignature = MEMSYSTEM_SIGNATURE;
	pHeader->pHeap = pHeap;
	pHeader->iSize = size;	
	pHeader->_prev = pHeader->_next = nullptr;

	if (pHeader->pHeap->_prevAddress)
	{
		pHeader->_next = nullptr;
		auto *copy = static_cast<AllocHeader*>(pHeader->pHeap->_prevAddress);
		pHeader->_prev = copy;
		copy->_next = pHeader;
	}

	pHeader->pHeap->_prevAddress = pHeader; // Points at itself

	auto *pStartMemBlock = pMem + sizeof(AllocHeader);
	auto*pEndMarker = reinterpret_cast<int*>(static_cast<char*>(pStartMemBlock) + size);
	*pEndMarker = MEMSYSTEM_ENDMARKER;

	pHeap->AddAllocation(iRequestedBytes);
	// memPool.Alloc(pMem, iRequestedBytes);

	return pStartMemBlock;
}

void * operator new(size_t size)
{
 	return operator new(size, HeapFactory::GetDefaultHeap());
}

void operator delete(void * pMem)
{
	auto *pHeader = reinterpret_cast<AllocHeader*>(static_cast<char*>(pMem) - sizeof(AllocHeader));

	const auto heap		= pHeader->pHeap;
	const auto size		= pHeader->iSize;
	const auto sig		= pHeader->iSignature;
	const auto next		= pHeader->_next;
	const auto prev		= pHeader->_prev;

	assert(sig == MEMSYSTEM_SIGNATURE);
	
	if (next)	
		next->_prev = prev;

	if (prev)	
		prev->_next = next;			

	if (!prev && !next)
		heap->_prevAddress = nullptr;

	if (!next)
		heap->_prevAddress = prev;

	auto *pEndMarker = static_cast<int*>(reinterpret_cast<int*>(static_cast<char*>(pMem) + size));

	assert(*pEndMarker == MEMSYSTEM_ENDMARKER);

	heap->RemoveAllocation(size + sizeof(AllocHeader) + sizeof(int));
	// memPool.FreeMemory(pMem, size);

	free(pHeader);
}

void operator delete[](void* pMem) noexcept
{
	operator delete(pMem);
}

