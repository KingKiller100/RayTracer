#pragma once
#include "rapidjson/document.h"
#include "FrameList.h"
#include <fstream>
#include <vector>
#include <iostream>
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
		std::vector<char*> lines;
		char* text;
		std::ifstream jsonFile(path, std::ifstream::in);

		

		if (jsonFile.is_open())
		{
			while (std::getline(jsonFile, std::string(text)))
			{
				lines.emplace_back(text);
			}
			jsonFile.close();
		}
		else
		{
			std::cerr << "Unable to open file sphere data.JSON";
			return;
		}

		Document doc;

		int fileLineSize = lines.size();

		for (int i = 0; i < fileLineSize; i++)
		{
			doc.Parse(lines[i]);


		}

	}

};
