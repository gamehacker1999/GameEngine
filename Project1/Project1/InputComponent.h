#pragma once
#include "MoveComponent.h"
#include"Actor.h"
class InputComponent :
	public MoveComponent
{
public:
	InputComponent(class Actor* owner);
	void ProcessInput(const Uint8* state) override;
	~InputComponent();

private:
	float maxForwardSpeed;
	float maxAngularSpeed;

	int forwardKey;
	int backwardKey;
	int clockwiseKey;
	int counterClockwiseKey;
};

