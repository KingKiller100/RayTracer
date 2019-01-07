#pragma once
#include "FrameList.h"
#include <vector>

class Animation
{
public:
	std::vector<FramesList*> animations;
	
	Animation()
	{

	}

	void ReadJSON();

};
