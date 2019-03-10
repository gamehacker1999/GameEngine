#pragma once
#include <vector>
#include<SDL.h>
#include "AnimSpriteComponent.h"



AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner,drawOrder)
{
	mCurrentFrame = 0;	
	mAnimFPS = 24;
}


AnimSpriteComponent::~AnimSpriteComponent()
{
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<Texture* >&textures)
{
	//Setting the textures and passing the current texture
	mAnimTextures = textures;
	mCurrentFrame = 0;
	SetTexture(mAnimTextures[0]);
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	if (mAnimTextures.size() > 0)
	{
		//update the current frame based on the frame rate and delta time
		mCurrentFrame += mAnimFPS * deltaTime;

		//wrap current frame as needed
		if (mCurrentFrame >= mAnimTextures.size())
		{
			mCurrentFrame -= mAnimTextures.size();
		}

		//Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrentFrame)]);
	}
}

void AnimSpriteComponent::Draw(Shader* shader)
{
	SpriteComponent::Draw(shader);
}