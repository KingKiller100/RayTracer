#pragma once
#include <map>

class MemoryPool
{
public:
	MemoryPool();
	void Initialize();
	~MemoryPool();

	static MemoryPool* Instance();

	void Alloc(const void *ptr, const size_t &iSize);
	void FreeMemory(void *p, size_t i_size);
	void Status();

private:
	static MemoryPool* _instance;
	std::map<const void*, size_t> poolMap;
	size_t totalMemorySize;
	int noOfPtrs;
};

