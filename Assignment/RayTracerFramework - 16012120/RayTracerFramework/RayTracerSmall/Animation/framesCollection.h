#pragma once
// #include "rapidjson/document.h" // free, open source external library for JSON parsing/generator. RapidJSON by a Tencent company - http://rapidjson.org/index.html
// #include "rapidjson/istreamwrapper.h" // http://rapidjson.org/index.html
#include "../include/nlohmann/json.hpp" // JSON for Modern C++ by Niels Lohmann - https://github.com/nlohmann/json#arbitrary-types-conversions
#include "FrameList.h"
#include <vector>

using namespace nlohmann;

class FramesCollection : public MemoryOverride<FramesCollection>
{
public:
	static Heap* _heap;

	std::vector<FramesList*> framesCollection;
	int numOfObjects;

	FramesCollection() = default;

	~FramesCollection();

	void ReadJSON(const char* path, unsigned frameNum);

	void UpdateFrame(Frame &frame, const int &frameNum, const int &maxframes);

private:
	Vec3f Interpolate(const Vec3f &next, const Vec3f &prev, const int &deltaFrames, const int &timerChange);
	float Interpolate(const float &next, const float &prev, const int &deltaFrames, const int &timerChange);

};
