#pragma once
// #include "rapidjson/document.h" // free, open source external library for JSON parsing/generator. RapidJSON by a Tencent company - http://rapidjson.org/index.html
// #include "rapidjson/istreamwrapper.h" // http://rapidjson.org/index.html
#include "include/nlohmann/json.hpp" // JSON for Modern C++ by Niels Lohmann - https://github.com/nlohmann/json#arbitrary-types-conversions
#include "FrameList.h"
#include <fstream>
#include <vector>
#include <strstream>

using namespace nlohmann;

class FramesCollection
{
public:
	std::vector<FramesList*> framesCollection;
	
	FramesCollection() = default;

	~FramesCollection()
	{
		for (FramesList* frames_list : framesCollection)
		{
			delete frames_list;
			frames_list = nullptr;
		}

		framesCollection.clear();
	}

	void ReadJSON(const char* path, unsigned frameNum)
	{
		std::ifstream file(path, std::ifstream::in);
		
		json json = json::parse(file);

		unsigned jsonSize = json.size();


		for (int i = 0; i < jsonSize; i++)
			framesCollection.emplace_back(new FramesList(frameNum));

		for (unsigned i = 0; i < jsonSize; i++)
		{
			int frameListSize = json[i]["framesList"].size();

			for (unsigned j = 0; j < frameListSize; j++)
			{
				auto f = Frame();

				f.objectID = json[i]["framesList"][j]["objectID"].get<std::string>();
				f.keyFrame = json[i]["framesList"][j]["keyFrame"].get<int>();
				f.scale = json[i]["framesList"][j]["scale"].get<double>();
				f.pos.x = json[i]["framesList"][j]["posX"].get<double>();
				f.pos.y = json[i]["framesList"][j]["posY"].get<double>();
				f.pos.z = -json[i]["framesList"][j]["posZ"].get<double>() - 20;
				f.brushColour.x = json[i]["framesList"][j]["brushR"].get<double>() * 0.003921568627451; // byte value divided by 255
				f.brushColour.y = json[i]["framesList"][j]["brushG"].get<double>() * 0.003921568627451; // byte value divided by 255
				f.brushColour.z = json[i]["framesList"][j]["brushB"].get<double>() * 0.003921568627451; // byte value divided by 255
				f.colour.x = json[i]["framesList"][j]["colourR"].get<double>();
				f.colour.y = json[i]["framesList"][j]["colourG"].get<double>();
				f.colour.z = json[i]["framesList"][j]["colourB"].get<double>();

				framesCollection[i]->framesList[j] = new Frame(f);
			}
		}
	}

	void ChangeSphereBetweenFrames(std::string objectID, int frameNum, int maxframes)
	{
		auto currentFrameList = framesCollection[(int)objectID[0] - 65]->framesList;
		int useableFrame = frameNum;


		if (currentFrameList[frameNum]->keyFrame == -1)
		{
			while (currentFrameList[useableFrame]->keyFrame == -1 && useableFrame < currentFrameList.size() - 1)
			{
				useableFrame++;
			}

			auto nextKeyFrame = currentFrameList[useableFrame];

			useableFrame = frameNum;

			while (currentFrameList[useableFrame]->keyFrame == -1 && useableFrame >= 0)
			{
				useableFrame--;
			}

			auto prevKeyFrame = currentFrameList[useableFrame];

			const int timerChange = frameNum - prevKeyFrame->keyFrame;
			const int deltaFrames = nextKeyFrame->keyFrame - prevKeyFrame->keyFrame;

			auto* currentFrame = currentFrameList[frameNum];

			if (frameNum <= nextKeyFrame->keyFrame && frameNum > prevKeyFrame->keyFrame)
			{
				currentFrame->pos = Interpolate(nextKeyFrame->pos, prevKeyFrame->pos, deltaFrames, timerChange);
				currentFrame->scale = Interpolate(nextKeyFrame->scale, prevKeyFrame->scale, deltaFrames, timerChange);
				currentFrame->brushColour = Interpolate(nextKeyFrame->brushColour, prevKeyFrame->brushColour, deltaFrames, timerChange);
				currentFrameList[frameNum] = currentFrame;
			}
		}
	}

private:
	Vec3f Interpolate(const Vec3f &next, const Vec3f &prev, const int &deltaFrames, const int &timerChange)
	{
		const Vec3f difference = next - prev;

		const float dfInv = float(1.0f / float(deltaFrames));

		const auto increment = prev + (difference * timerChange) * dfInv;

		return increment;
	}

	float Interpolate(const float &next, const float &prev, const int &deltaFrames, const int &timerChange)
	{
		const float difference = next - prev;

		const float dfInv = float(1.0f / float(deltaFrames));

		const float increment = prev + (difference * timerChange) * dfInv;

		return increment;
	}
};
