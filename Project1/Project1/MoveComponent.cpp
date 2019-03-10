#include "MoveComponent.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder )
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
		Quaternion rot = mOwner->GetRotation();
		float angle = angularSpeed * deltaTime;
		//create quaternion for rotation about up axis
		Quaternion inc = Quaternion(Vector3::UnitZ, angle);
		//concatenate rot and inc
		Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	if (forwardSpeed != DBL_MIN)
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetPosition()*forwardSpeed*deltaTime;
		mOwner->SetPosition(pos);
	}
}

MoveComponent::~MoveComponent()
{
}
