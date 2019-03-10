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

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	Texture* GetTexture(std::string file);

	//Texture* LoadTexture(std::string filename);

	bool LoadShaders();

	~Game();

private:
	//Helper functions for game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();



	//Window created by SDL
	SDL_Window* mWindow;
	//Renderer
	SDL_Renderer* mRenderer;
	//open gl context
	SDL_GLContext context;
	//game should continue to run
	bool mIsRunning;


	float mTickerCount;

	int mPaddleDir;

	//actor fields
	std::vector < class Actor* > mActors;
	std::vector < class  Actor* > mPendingActors;
	bool mUpdatingActors;

	//Texture fields 
	std::map<std::string,Texture*> textures;

	//Function to load all data
	void LoadData();
	void UnloadData();

	//Initialize sprite verts
	VertexArray *spriteVerts;
	float vertexBuffer[4];
	unsigned int indexBuffer[6];
	void InitSpriteVerts();

	//Fields and functions for sprites
	std::vector <class SpriteComponent*> mSprites;

	//Ship
	class Ship* mShip;
	Shader* spriteShader;

	
};

