#pragma once
#include "Component.h"
#include "Actor.h"
class MoveComponent :
	public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder=10);
	void Update(float deltaTime) override;
	float GetAngularSpeed() const { return angularSpeed; }
	float GetForwardSpeed() const { return forwardSpeed; }
	float SetAngularSpeed(float speed) { this->angularSpeed = speed; }
	float SetForwardSpeed(float speed) { this->forwardSpeed = speed; }

	virtual ~MoveComponent();

private:
	float angularSpeed;
	float forwardSpeed;
};

