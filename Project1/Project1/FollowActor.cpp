
#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"

FollowActor::FollowActor(Game* game)
	:Actor(game)
	, mMoving(false)
{
	meshComp = new SkeletalMeshComponent(this);
	meshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/CatWarrior.gpmesh"));
	meshComp->SetSkeleton(game->GetSkeleton("Assets/CatWarrior.gpskel"));
	meshComp->PlayAnimation(game->GetAnimation("Assets/CatActionIdle.gpanim"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

	moveComp = new MoveComponent(this);
	cameraComp = new FollowCamera(this);
	cameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::Pi;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::Pi;
	}

	// Did we just start moving?
	if (!mMoving && !Math::NearZero(forwardSpeed))
	{
		mMoving = true;
		meshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatRunSprint.gpanim"), 1.25f);
	}
	// Or did we just stop moving?
	else if (mMoving && Math::NearZero(forwardSpeed))
	{
		mMoving = false;
		meshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
	}
	moveComp->SetForwardSpeed(forwardSpeed);
	moveComp->SetAngularSpeed(angularSpeed);
}

void FollowActor::SetVisible(bool visible)
{
	 //meshComp->SetVisible(visible);
}

