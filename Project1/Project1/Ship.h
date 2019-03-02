#pragma once
#include"Actor.h"
#include "Game.h"
class Ship: public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const Uint8* state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
private:
	float mRightSpeed=0.0f;
	float mDownSpeed=0.0f;
};

