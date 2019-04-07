#pragma once
#include<SDL.h>
#include<map>
#include<vector>
#include "Vector2.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include"VertexArray.h"
#include "Shader.h"
#include"Texture.h"
#include"Renderer.h"
#include"CameraActor.h"
#include"FPSActor.h"
class Game
{
public:
	Game();
	//Initialize the game
	bool Start();
	//Game loop
	void Update();
	//Shutdown game
	void Shutdown();

	//Adding removing actor
	void AddActor(class Actor* actor);
	void RemoveActor(class  Actor* actor);

	class Renderer* GetRenderer() const { return renderer; }

	~Game();

private:
	//Helper functions for game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	//game should continue to run
	bool mIsRunning;


	float mTickerCount;

	int mPaddleDir;

	//actor fields
	std::vector < class Actor* > mActors;
	std::vector < class  Actor* > mPendingActors;
	bool mUpdatingActors;

	//Function to load all data
	void LoadData();
	void UnloadData();

	//Initialize sprite verts
	float vertexBuffer[4];
	unsigned int indexBuffer[6];
	//Fields and functions for sprites
	//Ship
	class Ship* mShip;
	class CameraActor* cameraActor;
	class FPSActor* fpsActor;
	//Renderer
	class Renderer* renderer;
	
};

