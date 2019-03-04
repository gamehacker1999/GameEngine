#pragma once

#include "Component.h"
#include "Actor.h"


Component::Component(Actor* owner, int updateOrder)
{
	mOwner = owner;
	mUpdateOrder = updateOrder;
	mOwner->AddComponent(this);
}


Component::~Component()
{
	mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{

}

void Component::ProcessInput(const Uint8 * state)
{
}
