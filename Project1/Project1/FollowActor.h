#pragma once
#include "Actor.h"
#include"FollowCamera.h"
#include"MoveComponent.h"
class FollowActor : public Actor
{
public:
	FollowActor(class Game* game);

	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);
private:
	MoveComponent* moveComp;
	FollowCamera* cameraComp;
	class SkeletalMeshComponent* meshComp;
	bool mMoving;
};
