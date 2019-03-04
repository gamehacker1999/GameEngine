#include "CircleComponent.h"



CircleComponent::CircleComponent(class Actor* owner):Component(owner)
{

}

float CircleComponent::GetRadius() const
{
	return radius;
}

Vector2 CircleComponent::GetCenter() const
{
	return mOwner->GetPosition();
}
