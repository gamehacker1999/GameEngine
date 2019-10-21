#include "FollowCamera.h"
#include"Game.h"


FollowCamera::FollowCamera(class Actor* actor):CameraComponent(actor)
{
	horizontalDistance = 300;
	verticalDistance = 300;
	targetDistance = 100;
	velocity = Vector3::Zero;
	springConstant = 64;
	SnapToIdeal();
}

FollowCamera::~FollowCamera()
{

}

Vector3 FollowCamera::ComputeCameraPosition()
{
	//computing the position of the camera with respect to the owner
	Vector3 position = mOwner->GetPosition() - (horizontalDistance*mOwner->GetForward())
		+ (verticalDistance*Vector3::UnitZ);

	return position;
}

void FollowCamera::Update(float deltaTime)
{
	//compute dampning from spring constant
	float dampning = 2.0 * sqrt(springConstant);

	//getting the ideal position of the camera
	Vector3 idealPosition = ComputeCameraPosition();

	Vector3 diff = actualPosition - idealPosition;

	Vector3 acel = -springConstant * diff - (dampning*velocity);

	velocity += acel * deltaTime;

	actualPosition += velocity *= deltaTime;

	//calculating target that is slighly in from of the actor
	Vector3 target = mOwner->GetPosition() + (mOwner->GetForward()*targetDistance);
	Vector3 up = Vector3::UnitZ;
	Vector3 position = ComputeCameraPosition();

	Matrix4 view = Matrix4::CreateLookAt(actualPosition, target, up);

	//setting up the view matrix
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
}


//function that snaps the camera to the ideal position at the beginning of the game
void FollowCamera::SnapToIdeal()
{
	actualPosition = ComputeCameraPosition();
	velocity = Vector3::Zero;
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward()*targetDistance;
	Matrix4 view = Matrix4::CreateLookAt(actualPosition, target, Vector3::UnitZ);
}
