#pragma once
#include "Actor.h"
#include"Game.h"
#include"SDL.h"
#include"MoveComponent.h"
class CameraActor :
	public Actor
{
public:
	CameraActor(class Game* game);
	~CameraActor();
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

private:
	MoveComponent* moveComponent;
};

