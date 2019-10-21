#pragma once
#include "Actor.h"
#include"Game.h"
class PlaneActor :
	public Actor
{
public:
	PlaneActor(class Game* game);
	~PlaneActor();
};

