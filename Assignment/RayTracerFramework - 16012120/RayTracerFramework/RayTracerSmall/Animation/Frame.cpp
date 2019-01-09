#include "Frame.h" 

Heap *Frame::_heap = nullptr;

Frame::Frame() : objectID(" "), keyFrame(-1), scale(1.f), pos(0), brushColour(255), colour(255)
{ }
