#include "PlaneActor.h"
#include"MeshComponent.h"


PlaneActor::PlaneActor(class Game* game):Actor(game)
{
	SetScale(10);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh"));
}


PlaneActor::~PlaneActor()
{
}
