#include "framesCollection.h"
#include <fstream>
#include <strstream>

Heap* FramesCollection::_heap = nullptr;

FramesCollection::~FramesCollection()
{
	for (FramesList* frames_list : framesCollection)
	{
		delete frames_list;
		frames_list = nullptr;
	}

	framesCollection.clear();
}

void FramesCollection::ReadJSON(const char* path, unsigned frameNum)
{
	std::ifstream file(path, std::ifstream::in);

	json json = json::parse(file);

	numOfObjects = json.size();
	
	for (unsigned i = 0; i < numOfObjects; i++)
	{
		framesCollection.emplace_back(new FramesList(frameNum));
		
		int frameListSize = json[i]["framesList"].size();

		for (unsigned j = 0; j < frameListSize; j++)
		{
			auto f = Frame();

			f.objectID = json[i]["framesList"][j]["objectID"].get<std::string>();
			f.keyFrame = json[i]["framesList"][j]["keyFrame"].get<int>();
			f.scale = json[i]["framesList"][j]["scale"].get<double>();
			f.pos.x = json[i]["framesList"][j]["posX"].get<double>();
			f.pos.y = json[i]["framesList"][j]["posY"].get<double>();
			f.pos.z = -json[i]["framesList"][j]["posZ"].get<double>() - 30;
			f.brushColour.x = json[i]["framesList"][j]["brushR"].get<double>() * 0.003921568627451; 
			f.brushColour.y = json[i]["framesList"][j]["brushG"].get<double>() * 0.003921568627451; 
			f.brushColour.z = json[i]["framesList"][j]["brushB"].get<double>() * 0.003921568627451; 
			f.colour.x = json[i]["framesList"][j]["colourR"].get<double>() * 0.003921568627451;		
			f.colour.y = json[i]["framesList"][j]["colourG"].get<double>() * 0.003921568627451;		
			f.colour.z = json[i]["framesList"][j]["colourB"].get<double>() * 0.003921568627451;		

			framesCollection[i]->framesList[j] = new Frame(f);
		}
	}
}

void FramesCollection::UpdateFrame(Frame &frame, const int &frameNum, const int &maxframes)
{
	auto currentFrameList = framesCollection[(int)frame.objectID[0] - 65]->framesList;
	int useableFrame = frameNum;


	if (currentFrameList[frameNum]->keyFrame == -1)
	{
		while (currentFrameList[useableFrame]->keyFrame == -1 && useableFrame < currentFrameList.size() - 1)
		{
			useableFrame++;
		}

		const auto nextKeyFrame = currentFrameList[useableFrame];

		useableFrame = frameNum;

		while (currentFrameList[useableFrame]->keyFrame == -1 && useableFrame >= 0)
		{
			useableFrame--;
		}

		const auto prevKeyFrame = currentFrameList[useableFrame];

		const int timerChange = frameNum - prevKeyFrame->keyFrame;
		const int deltaFrames = nextKeyFrame->keyFrame - prevKeyFrame->keyFrame;

		if (frameNum <= nextKeyFrame->keyFrame && frameNum > prevKeyFrame->keyFrame)
		{
			frame.pos = Interpolate(nextKeyFrame->pos, prevKeyFrame->pos, deltaFrames, timerChange);
			frame.scale = Interpolate(nextKeyFrame->scale, prevKeyFrame->scale, deltaFrames, timerChange);
			frame.brushColour = Interpolate(nextKeyFrame->brushColour, prevKeyFrame->brushColour, deltaFrames, timerChange);
			*currentFrameList[frameNum] = frame;
		}
	}
}

Vec3f FramesCollection::Interpolate(const Vec3f &next, const Vec3f &prev, const int &deltaFrames, const int &timerChange) const
{
	const Vec3f difference = next - prev;

	const float dfInv = float(1.0f / float(deltaFrames));

	const auto increment = prev + (difference * timerChange) * dfInv;

	return increment;
}

Vec3c FramesCollection::Interpolate(const Vec3c &next, const Vec3c &prev, const int &deltaFrames, const int &timerChange) const
{
	const Vec3c difference = next - prev;

	const float dfInv = float(1.0f / float(deltaFrames));

	const auto increment = prev + (difference * timerChange) * dfInv;

	return increment;
}

float FramesCollection::Interpolate(const float &next, const float &prev, const int &deltaFrames, const int &timerChange) const
{
	const float difference = next - prev;

	const float dfInv = float(1.0f / float(deltaFrames));

	const float increment = prev + (difference * timerChange) * dfInv;

	return increment;
}
