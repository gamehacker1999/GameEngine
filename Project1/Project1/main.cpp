#pragma once

#include <SDL.h>
#include <iostream>
#include "Game.h"

int main(int argc, char* argv[])
{
	Game game;

	bool success = game.Start();
	if (success)
	{
		game.Update();
	}
	game.Shutdown();
	return 0;
}