#pragma once
#include "Actor.h"
#include"Game.h"
class WallActor :
	public Actor
{
public:
	WallActor(class Game* game);
	~WallActor();
};

