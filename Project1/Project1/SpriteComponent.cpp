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
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}


SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	mTexHeight = texture->GetHeight();
	mTexWidth = texture->GetWidth();
}

void SpriteComponent::Draw(Shader* shader)
{

	//OpenGl implementation of Draw

	if (mTexture)
	{
		//Creating a scale matrix for the sprite size
		Matrix4 scaleMat = Matrix4::CreateScale(Vector3((float)mTexWidth, (float)mTexHeight, 0));

		//Multiplying this with the owning actor's transform
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		//Setting the transform
		shader->SetMatrixUniform("uWorldTransform", world);

		mTexture->SetActive();

		//Drawing the sprite
		glDrawElements(GL_TRIANGLES, //Kinds of polygon
			6, //Number of verts
			GL_UNSIGNED_INT, //Value of index buffer
			nullptr);
	}
}
