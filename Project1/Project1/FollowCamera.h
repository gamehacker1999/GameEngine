#pragma once
#include "CameraComponent.h"
#include"Actor.h"
#include"Math.h"
//camera that follows an actor
class FollowCamera :
	public CameraComponent
{
public:
	FollowCamera(class Actor* actor);
	~FollowCamera();
	Vector3 ComputeCameraPosition();
	void Update(float deltaTime) override;
	void SnapToIdeal();

private:
	//horizontal distance behind actor
	float horizontalDistance;
	//vertical distance abobe camera actor
	float verticalDistance;
	//target position, a little in front of the actor
	float targetDistance;

	//adding springiness to the camera
	float springConstant;
	Vector3 velocity;
	Vector3 actualPosition;
};

