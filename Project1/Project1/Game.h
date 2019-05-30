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
#include"Car.h"
#include"FollowActor.h"
#include"Skeleton.h"
#include"Animation.h"
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

	class Skeleton* GetSkeleton(const std::string& fileName);

	class Animation* GetAnimation(const std::string& fileName);

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

	//map of skeletons
	std::map<std::string, class Skeleton*> mSkeletons;

	//map of animations
	std::map<std::string, class Animation*> mAnimations;

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
	class Car* car;
	class FollowActor* followActor;

	//Renderer
	class Renderer* renderer;
	
};

