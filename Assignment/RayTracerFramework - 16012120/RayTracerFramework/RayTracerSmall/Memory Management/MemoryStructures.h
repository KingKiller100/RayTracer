#include "Heap.h"
#include "HeapFactory.h"
#include "MemoryPool.h"

// static MemoryPool memPool;

void * operator new(size_t size, Heap * pHeap);
void * operator new(size_t size);
void operator delete(void * pMem);
void operator delete[](void * pMem);