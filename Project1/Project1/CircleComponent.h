#pragma once
#include "Component.h"
#include"Actor.h"
class CircleComponent :
	public Component
{
public:
	CircleComponent(class Actor* owner);
	void SetRadius(float radius) { this->radius = radius; }
	float GetRadius() const;
	Vector3 GetCenter() const;

private:
	float radius;
};

