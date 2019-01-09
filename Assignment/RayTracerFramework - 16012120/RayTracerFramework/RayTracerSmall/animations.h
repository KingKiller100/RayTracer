#pragma once
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "FrameList.h"
#include <fstream>
#include <vector>
#include <string>

using namespace rapidjson;

class Animation
{
public:
	std::vector<FramesList*> framesCollection;
	
	Animation()
	{

	}

	void ReadJSON(char* path)
	{
		std::ifstream jsonFile(path, std::ifstream::in);

		IStreamWrapper isw(jsonFile);
		
		Document doc;
		doc.ParseStream(isw);
	}

};
