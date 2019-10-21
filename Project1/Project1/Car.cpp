#include "Car.h"


Car::Car(class Game* game):Actor(game)
{
	moveComp = new MoveComponent(this);
	carModel = new MeshComponent(this);

	//setting up the car model
	carModel->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));

	camera = new FollowCamera(this);
}


Car::~Car()
{
}

void Car::UpdateActor(float deltaTime)
{
}

void Car::ActorInput(const uint8_t* keys)
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
		angularSpeed -= Math::PiOver2;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::PiOver2;
	}


	moveComp->SetAngularSpeed(angularSpeed);
	moveComp->SetForwardSpeed(forwardSpeed);
}
