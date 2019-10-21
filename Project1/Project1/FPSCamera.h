#pragma once
#include "CameraComponent.h"
class FPSCamera :
	public CameraComponent
{
public:
	FPSCamera(class Actor* actor);
	void Update(float deltaTime) override;
	void SetPitchSpeed(float speed) { pitchSpeed = speed; }
	~FPSCamera();
private:
	//speed of pitch
	float pitchSpeed;
	//Max pitch value
	float maxPitch;
	//current pitch
	float pitch;
};

