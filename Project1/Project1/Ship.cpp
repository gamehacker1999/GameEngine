
#include "Actor.h"
#include "Ship.h"
#include "AnimSpriteComponent.h"
#include"InputComponent.h"
#include"MoveComponent.h"
#include <SDL.h>
#include<vector>


Ship::Ship(Game* game):Actor(game)
{
	mDownSpeed = 0;
	mRightSpeed = 0;
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<Texture*> anims = {
		GetGame()->GetRenderer()->GetTexture("Assets/Ship01.png"),
		GetGame()->GetRenderer()->GetTexture("Assets/Ship02.png"),
		GetGame()->GetRenderer()->GetTexture("Assets/Ship03.png"),
		GetGame()->GetRenderer()->GetTexture("Assets/Ship04.png"),
	};
	asc->SetAnimTextures(anims);

	float xPos = GetPosition().x+asc->GetTexWidth();
	float yPos = GetPosition().y-asc->GetTexHeight();

	SetPosition(Vector3(xPos, yPos,0));

    MoveComponent* ic = new MoveComponent(this);
}

void Ship::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector3 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;


	SetPosition(pos);
}

void Ship::ProcessKeyboard(const Uint8* state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	// right/left
	if (state[SDL_SCANCODE_D])
	{
		mRightSpeed += 250.0f;
	}
	if (state[SDL_SCANCODE_A])
	{
		mRightSpeed -= 250.0f;
	}
	// up/down
	if (state[SDL_SCANCODE_S])
	{
		mDownSpeed -= 300.0f;
	}
	if (state[SDL_SCANCODE_W])
	{
		mDownSpeed += 300.0f;
	}
}

