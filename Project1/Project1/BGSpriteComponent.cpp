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

void BGSpriteComponent::SetBGTextures(const std::vector<Texture* >&textures)
{
	//Setting background textures
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mTexture = tex;
		temp.mOffset.vx = count * mScreenSize.vx+1;
		temp.mOffset.vy = 0;
		mBGTextures.emplace_back(temp);
		SpriteComponent::SetTexture(tex);
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
			bg.mOffset.vx = (mBGTextures.size() - 1)*mScreenSize.vx-5;
		}
	}

}

void BGSpriteComponent::Draw(Shader* shader)
{
	for (auto &tex : mBGTextures)
	{
		if (tex.mTexture)
		{
			Matrix4 scaleMat = Matrix4::CreateScale(Vector3((float)mTexWidth, (float)mTexHeight, 0));

			auto mat = tex.BackgroundWorldTransform();

			//Multiplying this with the owning actor's transform
			Matrix4 world = scaleMat * mat;

			//Setting the transform
			shader->SetMatrixUniform("uWorldTransform", world);

			tex.mTexture->SetActive();

			//Drawing the sprite
			glDrawElements(GL_TRIANGLES, //Kinds of polygon
				6, //Number of verts
				GL_UNSIGNED_INT, //Value of index buffer
				nullptr);
		}
	}
}
