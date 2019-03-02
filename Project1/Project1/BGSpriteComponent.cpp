#pragma once
#include<vector>
#include<SDL.h>
#include "BGSpriteComponent.h"
#include "Actor.h"



BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner,drawOrder)
{
	mScrollSpeed = 0;
}


BGSpriteComponent::~BGSpriteComponent()
{
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture* >&textures)
{
	//Setting background textures
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mTexture = tex;
		temp.mOffset.vx = count * mScreenSize.vx;
		temp.mOffset.vy = 0;
		mBGTextures.emplace_back(temp);
		count++;
	}
}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	for (auto &bg : mBGTextures)
	{
		//Update the x offset
		bg.mOffset.vx += mScrollSpeed * deltaTime;
		//if this is completely offscreen reset it to the right of the previous BG
		if (bg.mOffset.vx < -mScreenSize.vx)
		{
			bg.mOffset.vx = (mBGTextures.size() - 1)*mScreenSize.vx - 1;
		}
	}

}

void BGSpriteComponent::Draw(SDL_Renderer* renderer)
{
	// Draw each background texture
	for (auto& bg : mBGTextures)
	{
		SDL_Rect r;
		// Assume screen size dimensions
		r.w = static_cast<int>(mScreenSize.vx);
		r.h = static_cast<int>(mScreenSize.vy);
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().vx - r.w / 2 + bg.mOffset.vx);
		r.y = static_cast<int>(mOwner->GetPosition().vy - r.h / 2 + bg.mOffset.vy);

		// Draw this background
		SDL_RenderCopy(renderer,
			bg.mTexture,
			nullptr,
			&r
		);
	}
}
