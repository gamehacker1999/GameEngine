#pragma once
#include "Actor.h"
#include"FollowCamera.h"
#include"Game.h"
#include"SDL.h"
#include"MeshComponent.h"
#include"MoveComponent.h"
class Car :
	public Actor
{
public:
	Car(class Game* game);
	~Car();
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

private:
	MeshComponent* carModel;
	MoveComponent* moveComp;
	FollowCamera* camera;
};

