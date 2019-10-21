#pragma once
#include "Component.h"
#include "SDL.h"
#include "SDL_image.h"
#include"Shader.h"
#include"Texture.h"
class SpriteComponent :
	public Component
{
public:
	//lower draw order means further back
	SpriteComponent(class Actor* owner, int drawOrder=100);
	~SpriteComponent();

	virtual void Draw(Shader* shader);
	virtual void SetTexture(Texture* texture);

	//Getter and setters
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }


protected:
	//Texture to draw
	Texture* mTexture;
	
	//Draw order, width, height
	int mDrawOrder;
	int mTexWidth;
	int mTexHeight;
};

