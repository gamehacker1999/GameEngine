#pragma once
#include "Math.h"
//struct to represent the transoft of a bone in an animated mesh
struct BoneTransform
{
	Quaternion mRotation;
	Vector3 mTranslation;

	//convert to matrix
	Matrix4 ToMatrix() const;

	static BoneTransform Interpolate(const BoneTransform& start, const BoneTransform& end, float percent);

};

