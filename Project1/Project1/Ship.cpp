
#include "Actor.h"
#include "Ship.h"
#include "AnimSpriteComponent.h"
#include <SDL.h>
#include<vector>


Ship::Ship(Game* game):Actor(game)
{
	mDownSpeed = 0;
	mRightSpeed = 0;
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		GetGame()->GetTexture("Assets/Ship01.png"),
		GetGame()->GetTexture("Assets/Ship02.png"),
		GetGame()->GetTexture("Assets/Ship03.png"),
		GetGame()->GetTexture("Assets/Ship04.png"),
	};

	asc->SetAnimTextures(anims);
}

void Ship::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.vx += mRightSpeed * deltaTime;
	pos.vy += mDownSpeed * deltaTime;

	if (pos.vx < 25)
	{
		pos.vx = 25;
	}

	else if (pos.vx > 500.0f)
	{
		pos.vx = 500.0f;
	}
	if (pos.vy < 25.0f)
	{
		pos.vy = 25.0f;
	}
	else if (pos.vy > 743.0f)
	{
		pos.vy = 743.0f;
	}

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
		mDownSpeed += 300.0f;
	}
	if (state[SDL_SCANCODE_W])
	{
		mDownSpeed -= 300.0f;
	}
}

