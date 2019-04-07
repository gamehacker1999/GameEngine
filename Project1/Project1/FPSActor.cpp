#include "FPSActor.h"
#include"Game.h"
#include"SDL.h"
#include<iostream>


FPSActor::FPSActor(class Game* game):Actor(game)
{
	moveComp = new MoveComponent(this);
	fpsCamera = new FPSCamera(this);
}


FPSActor::~FPSActor()
{
}

void FPSActor::UpdateActor(float deltaTime)
{
}

void FPSActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 300.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 300;
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 300;
	}

	//getting the mouse state to calculate relative motion
	int x;
	int y;

	//getting right/left movement and pitch
	Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
	const int maxMouseSpeed = 500;
	const int maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0;
	if (x != 0)
	{
		//assuming max speed per frame is 500, converts x to [-1.0,1.0]
		angularSpeed=static_cast<float>(x)/maxMouseSpeed;
		//finding angular speed for frame;
		angularSpeed *= maxAngularSpeed;
	}
	float pitchSpeed = 0;
	if (y != 0)
	{
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		//claculating max pitch speed
		pitchSpeed *= maxAngularSpeed;
	}

	moveComp->SetAngularSpeed(angularSpeed);
	moveComp->SetForwardSpeed(forwardSpeed);
	moveComp->SetStrafeSpeed(strafeSpeed);
	fpsCamera->SetPitchSpeed(pitchSpeed);
}
