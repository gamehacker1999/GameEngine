#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const
{
	Matrix4 rot = Matrix4::CreateFromQuaternion(mRotation);
	Matrix4 trans = Matrix4::CreateTranslation(mTranslation);

	return rot * trans;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform & start, const BoneTransform & end, float percent)
{
	BoneTransform retVal;

	//using spherical linear interpolation to rotate this bone
	retVal.mRotation = Quaternion::Slerp(start.mRotation, end.mRotation, percent);

	//lerping the translation
	retVal.mTranslation = Vector3::Lerp(start.mTranslation, end.mTranslation, percent);

	return retVal;
}
