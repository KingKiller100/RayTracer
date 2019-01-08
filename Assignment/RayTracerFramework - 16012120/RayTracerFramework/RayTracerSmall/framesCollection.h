#pragma once
// #include "rapidjson/document.h" // free, open source external library for JSON parsing/generator. RapidJSON by a Tencent company - http://rapidjson.org/index.html
// #include "rapidjson/istreamwrapper.h" // http://rapidjson.org/index.html
#include "include/nlohmann/json.hpp" // JSON for Modern C++by Niels Lohmann - https://github.com/nlohmann/json#arbitrary-types-conversions
#include "FrameList.h"
#include <fstream>
#include <vector>
#include <iostream>

using namespace nlohmann;

class Animation
{
public:
	std::vector<FramesList*> framesCollection;
	
	Animation() = default;

	void ReadJSON(const char* path)
	{
		std::ifstream file("Data\\spheres1.json", std::ifstream::in);
		
		nlohmann::json j = nlohmann::json::parse(file);

		auto f = new Frame[j.size()];
		for (unsigned i = 0; i < j.size(); i++)
		{
			if (j["keyFrame"] == -1)
				continue;

			f[i].objectID = j["objectID"].get<char>();
			f[i].keyFrame = j["keyFrame"].get<int>();
			f[i].scale = j[i]["scale"].get<double>();
			f[i].pos.x = j[i]["posX"].get<double>();
			f[i].pos.y = j[i]["posY"].get<double>();
			f[i].pos.z = j[i]["posZ"].get<double>();
			f[i].brushColour.x = j[i]["brushR"].get<int>();
			f[i].brushColour.y = j[i]["brushG"].get<int>();
			f[i].brushColour.z = j[i]["brushB"].get<int>();
			f[i].colour.x = j[i]["colourR"].get<int>();
			f[i].colour.y = j[i]["colourG"].get<int>();
			f[i].colour.z = j[i]["colourB"].get<int>();
		}		
	}

};
