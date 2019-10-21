#pragma once
#include<string>
#include<vector>
#include"Math.h"
#include"BoneTransform.h"
class Animation
{
public:
	Animation();
	~Animation();

	bool Load(const std::string& fileName);

	int GetNumBones();

	int GetNumFrames();

	float GetDuration();

	float GetFrameDuration();

	//fills the provided vector with the global pose matrix of each bone
	//at a specified time in the animation

	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;

	void Update(float deltaTime);

private:

	//number of bones	
	int numBones;

	//number of frames
	int numFrames;

	//duration of animation
	float duration;

	//duration of each frame
	float frameDuration;

	//transform information of each bone
	//index in the outer vector is a bone and the inner vector is a frame
	std::vector<std::vector<BoneTransform>> tracks;
};

