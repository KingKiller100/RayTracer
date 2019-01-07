#include <cassert>
#include "MemoryPool.h"
#include "Heap.h"
#include <iostream>

MemoryPool* MemoryPool::_instance = nullptr;

MemoryPool* MemoryPool::Instance()
{
	if (!_instance)
		_instance = new MemoryPool;

	return _instance;
}

MemoryPool::MemoryPool() 
{
	Initialize();
}


void MemoryPool::Initialize()
{/*
	if (!_currentAddress)
		_currentAddress = memBlock;*/
		/*short* x = (short*)malloc(sizeof(short));
		poolMap.insert(std::pair<const void*, size_t>((void*)(x), sizeof(x)));

		poolMap.clear()*/;
	if (poolMap.size() <= 0)
	{
		noOfPtrs = NULL;
		totalMemorySize = NULL;
	}
}

MemoryPool::~MemoryPool()
{
	for (auto it = poolMap.begin(); it != poolMap.end(); ++it)
		if (it->first)
			poolMap.erase(it);

	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

void MemoryPool::Alloc(const void *ptr, const size_t &iSize)
{	
	poolMap.emplace(std::pair<const void*, size_t>(ptr, iSize));
	totalMemorySize += iSize + sizeof(AllocHeader) + sizeof(int);
	noOfPtrs++;
}

void MemoryPool::FreeMemory(void* pMem, size_t iSize)
{
	poolMap.erase(pMem);
	totalMemorySize -= iSize;
	noOfPtrs--;
}

void MemoryPool::Status()
{
	std::cout << "Total Ptrs " << noOfPtrs << "\n" << std::endl;
	std::cout << "Total Size In Memory Pool" << totalMemorySize << "\n" << std::endl;

	std::cout << "List of Pointers \n" << std::endl;
	for (auto& it : poolMap)
		std::cout << "Pointer Address: " << it.first << "Byte Size: " << it.second << std::endl;
}
