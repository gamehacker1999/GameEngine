#pragma once
#include "Actor.h"
#include"MoveComponent.h"
#include"FPSCamera.h"
class FPSActor :
	public Actor
{
public:
	FPSActor(class Game* game);
	~FPSActor();
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

private:
	MoveComponent* moveComp;
	FPSCamera* fpsCamera;
};

