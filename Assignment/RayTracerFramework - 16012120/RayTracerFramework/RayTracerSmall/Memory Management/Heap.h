#pragma once
#include <string>

class Heap
{
private:
	std::string m_name;
	size_t m_allocatedBytes;

public:
	Heap();
	Heap(std::string name);

	void Initialise(const std::string &n);	

	void SetName(const std::string &n)					{ m_name = n; }
	std::string GetName() const							{ return m_name; };

	void AddAllocation(size_t size);
	void RemoveAllocation(size_t size);
	size_t TotalAllocation() const						{ return m_allocatedBytes; }
};

