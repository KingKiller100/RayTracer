#include "HeapFactory.h"
#include <iostream>

Heap* HeapFactory::_defaultHeap = nullptr;
std::vector<Heap*> HeapFactory::heapContainer;

HeapFactory::HeapFactory() {}

HeapFactory::~HeapFactory()
{
	free(_defaultHeap);
}

Heap * HeapFactory::CreateHeap(const char * szName)
{
	Heap *newHeap = new Heap(szName);
	heapContainer.push_back(newHeap);

	std::cout << "Creating Heap: " << newHeap->GetName() << "\n" << std::endl;

	return newHeap;
}

void HeapFactory::CreateDefaultHeap()
{
	_defaultHeap = (Heap*)malloc(sizeof(Heap));
	_defaultHeap->Initialise("default");
	heapContainer.push_back(_defaultHeap);
}

Heap * HeapFactory::GetDefaultHeap()
{
	if (!_defaultHeap)
		CreateDefaultHeap();

	return _defaultHeap;
}