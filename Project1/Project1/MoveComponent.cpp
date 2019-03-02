#include "MoveComponent.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder=10 )
	:Component(owner,updateOrder)
{
	forwardSpeed = 0;
	angularSpeed = 0;
}

void MoveComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	if (angularSpeed != DBL_MIN)
	{
		float rot = mOwner->GetRotation();
		rot += angularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	if (forwardSpeed != DBL_MIN)
	{
		Vector2 pos = mOwner->GetPosition();
		pos += mOwner->GetPosition()*forwardSpeed*deltaTime;
		mOwner->SetPosition(pos);
	}
}

MoveComponent::~MoveComponent()
{
}
