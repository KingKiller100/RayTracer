#include "HeapFactory.h"
#include <iostream>

Heap* HeapFactory::_defaultHeap = nullptr;
std::vector<Heap*> HeapFactory::heapContainer;

HeapFactory::HeapFactory() {}

HeapFactory::~HeapFactory()
{
	if (_defaultHeap)
		free(_defaultHeap);

	for (Heap* heap : heapContainer)
		free(heap);
}

Heap * HeapFactory::CreateHeap(const char * szName)
{	
	Heap* newHeap = (Heap*)malloc(sizeof(Heap));
	newHeap->Initialize(szName);

	heapContainer.emplace_back(newHeap);	
	return newHeap;
}

Heap* HeapFactory::GetDefaultHeap()
{
	if (_defaultHeap)
		return _defaultHeap;

	_defaultHeap = (Heap*)malloc(sizeof(Heap));
	_defaultHeap->Initialize("default");

	//heapContainer.emplace_back(_defaultHeap);

	return _defaultHeap;
}

void HeapFactory::WalkTheHeap(const size_t &id)
{
	if (id == 0)
	{
		_defaultHeap->WalkHeap();
		return;
	}

	if (id - 1 < heapContainer.size())
		heapContainer.at(id - 1)->WalkHeap();
	else
		std::cout << "outside heap container scope \n" << std::endl;
}
