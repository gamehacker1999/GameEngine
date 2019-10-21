#include "CameraComponent.h"
#include"Game.h"


CameraComponent::CameraComponent(class Actor* owner):Component(owner)
{

}


CameraComponent::~CameraComponent()
{
}

void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	//pass view to game
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
}

void CameraComponent::SetSkyBoxMatrix(const Matrix4& skyboxView)
{
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetSkyboxView(skyboxView);
}

