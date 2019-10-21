#include "SkeletalMeshComponent.h"



SkeletalMeshComponent::SkeletalMeshComponent(class Actor* owner) :MeshComponent(owner, true)
{
	skeleton = new Skeleton();
	isSkeletal = true;
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
}

void SkeletalMeshComponent::Draw(Shader * shader)
{
	if (mesh)
	{
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
		shader->SetMatrixUniforms("uMatrixPalette", &matrixPallete.entry[0],96);
		Texture* t = mesh->GetTexture(meshIndex);
		if (t)
			t->SetActive();

		VertexArray* va = mesh->GetVertexArray();
		va->SetActive();

		shader->SetFloatUniform("specularPower", mesh->GetSpecularPower());

		//draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (animation && skeleton)
	{
		animTime += deltaTime * animationRate;
		// Wrap around anim time if past duration
		while (animTime > animation->GetDuration())
		{
			animTime -= animation->GetDuration();
		}

		// Recompute matrix palette
		ComputeMatrixPallete();
	}
}

void SkeletalMeshComponent::SetSkeleton(Skeleton * sk)
{
	skeleton = sk;
}

void SkeletalMeshComponent::ComputeMatrixPallete()
{
	const std::vector<Matrix4>  globalInverseBindPoses = skeleton->GetGlobalInverseBindPose();

	std::vector<Matrix4> currentPoses;

	animation->GetGlobalPoseAtTime(currentPoses, skeleton, animTime);

	//setting the pallete of each bone
	for (size_t i = 0; i < skeleton->GetNumBones(); i++)
	{
		matrixPallete.entry[i] = globalInverseBindPoses[i] * currentPoses[i];
	}
}

//function to play animation
float SkeletalMeshComponent::PlayAnimation(Animation * anim, float playRate)
{

	animation = anim;

	animTime = 0;

	animationRate = playRate;

	if (!animation)
	{
		return 0;
	}

	ComputeMatrixPallete();

	return animation->GetDuration();
}
