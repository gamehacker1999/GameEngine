#include "FPSCamera.h"



FPSCamera::FPSCamera(class Actor* actor):CameraComponent(actor)
{
	pitch = 0;
	maxPitch = Math::ToRadians(60);
	pitchSpeed = 0;
}

void FPSCamera::Update(float deltaTime)
{
	//creating the view matrix using the look at format
	Vector3 pos = mOwner->GetPosition();

	pitch += pitchSpeed * deltaTime;

	//clamp the pitch to +/- of max value
	pitch = Math::Clamp(pitch, -maxPitch, maxPitch);
	//Make a quaternion representing pitch rotation
	//which is about the owner's right
	Quaternion rot(mOwner->GetRight(), pitch);

	//rotate owner forward by pitch rotation
	Vector3 forward = Vector3::Transform(mOwner->GetForward(), rot);


	Vector3 target = pos + forward * 100;

	//rotating the up vector
	Vector3 up = Vector3::Transform(Vector3::UnitZ, rot);

	//setting up view
	Matrix4 view = Matrix4::CreateLookAt(pos, target, up);
	SetViewMatrix(view);
}


FPSCamera::~FPSCamera()
{
}
