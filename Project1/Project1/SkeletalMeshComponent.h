#pragma once
#include "MeshComponent.h"
#include"Skeleton.h"
#include"Animation.h"
#include"Math.h"
class MeshComponent;

class SkeletalMeshComponent :
	public MeshComponent
{
	Skeleton* skeleton;

public:

	const size_t MAX_BONES = 96;

	struct MatrixPallete
	{
		Matrix4 entry[255];
	};

	SkeletalMeshComponent(class Actor* owner);
	~SkeletalMeshComponent();

	//draw this mesh component
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	//play animation
	void PlayAnimation(const std::string& file);

	void SetSkeleton(Skeleton* sk);

	void ComputeMatrixPallete();

	float PlayAnimation(Animation* anim, float playRate=1.0f);

private:

	//matrix pallete
	MatrixPallete matrixPallete;

	//current animation playing
	class Animation* animation;

	//play rate of animation
	float animationRate;

	//current time in animation
	float animTime;
};

