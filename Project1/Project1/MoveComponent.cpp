#include "MoveComponent.h"
#include"Math.h"
#include<iostream>

MoveComponent::MoveComponent(class Actor* owner, int updateOrder )
	:Component(owner,updateOrder)
{
	forwardSpeed = 0;
	angularSpeed = 0;
}

void MoveComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	if (!Math::NearZero(angularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		float angle = angularSpeed * deltaTime;
		//create quaternion for rotation about up axis
		Quaternion inc(Vector3::UnitZ, angle);
		//concatenate rot and inc
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	if (!Math::NearZero(forwardSpeed))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward()*forwardSpeed*deltaTime;
		mOwner->SetPosition(pos);
	}

	if (!Math::NearZero(forwardSpeed) || !Math::NearZero(strafeSpeed))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward()*forwardSpeed*deltaTime;
		//strafing
		pos += mOwner->GetRight()*strafeSpeed*deltaTime;
		mOwner->SetPosition(pos);
	}
}

MoveComponent::~MoveComponent()
{
}
