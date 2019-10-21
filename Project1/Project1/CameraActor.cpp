#include "CameraActor.h"
#include"Math.h"
#include<iostream>


CameraActor::CameraActor(class Game* game):Actor(game)
{
	moveComponent = new MoveComponent(this);
}


CameraActor::~CameraActor()
{

}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 100;
	Vector3 up = Vector3::UnitZ;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
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
		angularSpeed -= Math::Pi;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::Pi;
	}

	moveComponent->SetForwardSpeed(forwardSpeed);
	moveComponent->SetAngularSpeed(angularSpeed);
}
