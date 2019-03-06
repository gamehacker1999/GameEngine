#pragma once

#include "SpriteComponent.h"
#include<math.h>
#include "Actor.h"
#include "Game.h"
SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
{
	mDrawOrder = drawOrder;
	mTexture = nullptr;
	mDrawOrder = drawOrder;
	mTexWidth = 0;
	mTexHeight = 0;
	mOwner->GetGame()->AddSprite(this);
}


SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	SDL_QueryTexture(mTexture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

void SpriteComponent::Draw(Shader* shader)
{
	/*if (mTexture)
	{
		SDL_Rect r; //Rect to represent the sprite

		//Scaling the width and height of the sprite based on the scale of the owner actor
		r.w = static_cast<int>(mTexWidth*mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight*mOwner->GetScale());
		//centering the position of the rect
		r.x = static_cast<int>(mOwner->GetPosition().vx - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().vy - r.h / 2);

		//Draw the sprite
		SDL_RenderCopyEx(renderer,
			mTexture, //Texture to draw
			nullptr, // Source rect
			&r, // Destination rect
			mOwner->GetRotation()*180/M_PI, // Convert rotation radians to degrees
		    nullptr, //Point of rotation
			SDL_FLIP_NONE //Flip behavior
		);
	}*/
	glDrawElements(
		GL_TRIANGLES,//type of primitive to draw
		6,//Number of indices in the index buffer
		GL_UNSIGNED_INT,//Type of each index
		nullptr);// Usually nullptr
}
