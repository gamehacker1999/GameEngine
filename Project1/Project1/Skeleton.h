#pragma once
#include"Bone.h"
#include"BoneTransform.h"
#include<vector>
#include"Math.h"
#include<string>
class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	//load the skeleton data
	bool Load(const std::string& fileName);

	//accessors
	int GetNumBones();
	const Bone& GetBone(int index);
	const std::vector<Bone>& GetBones() const;
	const std::vector<Matrix4>& GetGlobalInverseBindPose();

protected:
	void ComputeGlobalInverseBindPose();

public:
	//bones in the skeleton
	std::vector<Bone> bones;
	//gloabal inverse bind pose for each bone
	std::vector<Matrix4> globalBindInversePose;
};

