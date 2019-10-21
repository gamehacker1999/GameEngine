#pragma once
#include "Vector2.h"
#include "SpriteComponent.h"
#include"Texture.h"
#include"Math.h"
class BGSpriteComponent :
	public SpriteComponent
{
public:
	BGSpriteComponent(Actor* owner, int drawOrder=10);

	//update draw overriden from parent
	void Draw(Shader* shader) override;
	void Update(float deltaTime) override;
	//Set textures for background
	void SetBGTextures(const std::vector<Texture* >&textures);

	//getters and setters
	void SetScreenSize(const class Vector2& size){ mScreenSize = size; }
	void SetScrollSpeed(float speed)  { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
	~BGSpriteComponent();

private:
	//Struct to encapsulate each bg image and its offset
	struct BGTexture
	{
		Texture* mTexture;
		Vector2 mOffset;

		Matrix4& BackgroundWorldTransform()
		{
			Matrix4 transform = Matrix4::CreateScale(1);
			transform *= Matrix4::CreateRotationZ(0);
			transform *= Matrix4::CreateTranslation(Vector3(mOffset.vx, mOffset.vy, 0));
			return transform;
		}
	};

	std::vector<BGTexture> mBGTextures;
	float mScrollSpeed;
	Vector2 mScreenSize;
};

