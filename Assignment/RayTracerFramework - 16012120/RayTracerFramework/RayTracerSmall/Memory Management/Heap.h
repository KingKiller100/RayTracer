#pragma once

#define MEMSYSTEM_SIGNATURE 0xdeadc0de //233495774
#define MEMSYSTEM_ENDMARKER 0xdeadbeef //218815710

class Heap
{
private:
	char* m_name;
	size_t m_allocatedBytes;

public:
	void *_prevAddress;

public:
	Heap();
	Heap(char* name);

	void Initialize(const char* n);

	void SetName(const char* n)						{ m_name = const_cast<char*>(n); }
	char* GetName() const							{ return m_name; };
	
	void AddAllocation(const size_t &size);
	void RemoveAllocation(const size_t &size);
	size_t TotalAllocation() const					{ return m_allocatedBytes; }
	
	void WalkHeap(); // Walks through the current AllocHeader
};

struct AllocHeader
{
	int iSignature;
	Heap *pHeap;
	int iSize;
	AllocHeader *_prev;
	AllocHeader *_next;
};

