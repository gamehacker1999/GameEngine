#pragma once
#include "CameraComponent.h"
//camera that follows an actor
class FollowCamera :
	public CameraComponent
{
public:
	FollowCamera();
	~FollowCamera();
};

