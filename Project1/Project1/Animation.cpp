#include "Animation.h"
#include"Skeleton.h"
#include"Bone.h"
#include<rapidjson/document.h>
#include<fstream>
#include<sstream>
#include<SDL.h>


Animation::Animation()
{
}


Animation::~Animation()
{
}

bool Animation::Load(const std::string & fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Animation %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Animation %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the metadata
	if (ver != 1)
	{
		SDL_Log("Animation %s unknown format", fileName.c_str());
		return false;
	}

	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject())
	{
		SDL_Log("Animation %s doesn't have a sequence.", fileName.c_str());
		return false;
	}

	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& bonecount = sequence["bonecount"];

	if (!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint())
	{
		SDL_Log("Sequence %s has invalid frames, length, or bone count.", fileName.c_str());
		return false;
	}

	numFrames = frames.GetUint();
	duration = length.GetDouble();
	numBones = bonecount.GetUint();
	frameDuration = duration / (numFrames - 1);

	tracks.resize(numBones);

	const rapidjson::Value& tracks = sequence["tracks"];

	if (!tracks.IsArray())
	{
		SDL_Log("Sequence %s missing a tracks array.", fileName.c_str());
		return false;
	}

	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		if (!tracks[i].IsObject())
		{
			SDL_Log("Animation %s: Track element %d is invalid.", fileName.c_str(), i);
			return false;
		}

		size_t boneIndex = tracks[i]["bone"].GetUint();

		const rapidjson::Value& transforms = tracks[i]["transforms"];
		if (!transforms.IsArray())
		{
			SDL_Log("Animation %s: Track element %d is missing transforms.", fileName.c_str(), i);
			return false;
		}

		BoneTransform temp;

		if (transforms.Size() < numFrames)
		{
			SDL_Log("Animation %s: Track element %d has fewer frames than expected.", fileName.c_str(), i);
			return false;
		}

		for (rapidjson::SizeType j = 0; j < transforms.Size(); j++)
		{
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];

			if (!rot.IsArray() || !trans.IsArray())
			{
				SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
				return false;
			}

			temp.mRotation.x = rot[0].GetDouble();
			temp.mRotation.y = rot[1].GetDouble();
			temp.mRotation.z = rot[2].GetDouble();
			temp.mRotation.w = rot[3].GetDouble();

			temp.mTranslation.x = trans[0].GetDouble();
			temp.mTranslation.y = trans[1].GetDouble();
			temp.mTranslation.z = trans[2].GetDouble();

			this->tracks[boneIndex].emplace_back(temp);
		}
	}

	return true;
}

int Animation::GetNumBones()
{
	return numBones;
}

int Animation::GetNumFrames()
{
	return numFrames;
}

float Animation::GetDuration()
{
	return duration;
}

float Animation::GetFrameDuration()
{
	return frameDuration;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton * inSkeleton, float inTime) const
{
	//resize the outpose to fit the num of bones
	if (outPoses.size() != numBones)
	{
		outPoses.resize(numBones);
	}

	//computing postion for frame;
	const int frame = static_cast<size_t>(inTime / frameDuration);
	int nextFrame = frame + 1;

	//calculate the ppercentage of time remaining to reach next frame
	float pct = inTime / frameDuration - frame;
	
	//set the pose for root
	//does the root have any animation
	if (tracks[0].size() > 0)
	{
		BoneTransform interp = BoneTransform::Interpolate(tracks[0][frame], tracks[0][nextFrame], pct);
		outPoses[0] = interp.ToMatrix();
	}

	else
	{
		outPoses[0] = Matrix4::Identity;
	}

	const std::vector<Bone>& bones = inSkeleton->GetBones();

	//calculating animation for each bone
	for (size_t i = 1; i < numBones; i++)
	{
		//Local marix
		Matrix4 localMat;

		if (tracks[i].size() > 0)
		{

			BoneTransform interp = BoneTransform::Interpolate(tracks[i][frame], tracks[i][nextFrame], pct);
			localMat = interp.ToMatrix();
		}

		outPoses[i] = localMat * outPoses[bones[i].parentIndex];
	}
}


