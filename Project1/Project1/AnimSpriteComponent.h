#pragma once
#include "SpriteComponent.h"
#include"Texture.h"
#include"Shader.h"
class AnimSpriteComponent :
	public SpriteComponent
{
public:
	AnimSpriteComponent(Actor* owner, int drawOrder=100);
	~AnimSpriteComponent();
	//Update animation every frame
	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;
	//Set the textures used for animation
	void SetAnimTextures(const std::vector<Texture* >&textures);
	//Set/Get anim fpf
	float GetAnimFps() const { return mAnimFPS; }
	void SetAnimFps(float fps) { mAnimFPS = fps; }

private:
	//All textures in the animation
	std::vector<Texture* > mAnimTextures;
	//Current frame
	float mCurrentFrame;
	//Animation FPS
	float mAnimFPS;
};

