#pragma once
#include"BoneTransform.h"
#include<string>
struct Bone
{
	//local transform of bone
	BoneTransform localBindPose;
	std::string name;
	int parentIndex;
};

