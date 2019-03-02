#include "InputComponent.h"
#include "SDL.h"



InputComponent::InputComponent(class Actor* owner):MoveComponent(owner)
{
	maxAngularSpeed = 5;
	maxForwardSpeed = 10;
	forwardKey = SDL_SCANCODE_W;
	backwardKey = SDL_SCANCODE_S;
	clockwiseKey = SDL_SCANCODE_D;
	counterClockwiseKey = SDL_SCANCODE_A;
}


InputComponent::~InputComponent()
{
}

void InputComponent::ProcessInput(const Uint8* state)
{
	Component::ProcessInput(state);

	float forwardSpeed = 0;
	if (state[forwardKey])
	{
		forwardSpeed += maxForwardSpeed;
	}

	if (state[backwardKey])
	{
		forwardSpeed -= maxForwardSpeed;
	}

	SetForwardSpeed(forwardSpeed);

	float angularSpeed = 0;

	if (state[clockwiseKey])
	{
		angularSpeed += maxAngularSpeed;
	}

	if (state[counterClockwiseKey])
	{
		angularSpeed -= maxAngularSpeed;
	}

	SetAngularSpeed(angularSpeed);
}
