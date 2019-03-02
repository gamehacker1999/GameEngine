#pragma once

#include "Game.h"
#include"Box2D\Box2D.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include <math.h>
#include<memory>
#include<SDL.h>
#include<SDL_image.h>

Game::Game()
{
	mTickerCount = 0;
}


Game::~Game()
{
}

bool Game::Start()
{
	//SDL initialize
	int sdlResult = SDL_INIT_EVERYTHING;
	if (sdlResult < 0)
	{
		SDL_Log("unable to to initialize SDL: &s", SDL_GetError());
		return false;
	}

	//creating sdl window
	mWindow = SDL_CreateWindow("Game Program", 100, 100, 1024, 769, 0);

	if (!mWindow)
	{
		SDL_Log("Window not created: &s", SDL_GetError());
		return false;
	}

	//Creating renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer)
	{
		SDL_Log("Window not created &s", SDL_GetError());
		return false;
	}

	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);

	mIsRunning = true;
	mTickerCount = SDL_GetTicks();

	LoadData();
	return true;
}

void Game::LoadData()
{
	//create a ship
	mShip = new Ship(this);

	//Create actor for background
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512, 384));

	//Create a far background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024, 768));
	std::vector<SDL_Texture* >bgtexts = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png"),

	};

	bg->SetBGTextures(bgtexts);
	bg->SetScrollSpeed(-100);

	//Create a near background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024, 768));
	bgtexts = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};	
	bg->SetBGTextures(bgtexts);
	bg->SetScrollSpeed(-200);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : textures)
	{
		SDL_DestroyTexture(i.second);
	}

	textures.clear();
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	IMG_Quit();

	//Checking and deleting all the actors
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	SDL_QUIT;
}

void Game::Update()
{
	while (mIsRunning)
	{
		UpdateGame();
		ProcessInput();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	//Creating and event object
	SDL_Event event;

	//Checking if the event queue has stuff in it
	while (SDL_PollEvent(&event))
	{
		//Checking against events
		switch (event.type)
		{
			//SDL quitting event
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	//Getting the keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);

	//If the user presses escape, sdl quit
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;

	mShip->ProcessKeyboard(state);
}

void Game::GenerateOutput()
{
	//Clear the back buffer
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}


	//Swap front and back buffers
	SDL_RenderPresent(mRenderer);
}

void Game::UpdateGame()
{
	//Wait until atleast 16 ms has passed until the last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTickerCount + 16));

	//Find delta time using get ticks which stores the
	//Number of milliseconds elapsed since init function call
	float deltaTime = SDL_GetTicks() - mTickerCount;
	//Dividing by thousand to get seconds elapsed
	deltaTime /= 1000.0f;

	//Clamping max delta time
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	//Storing the ticks value in mtickerCount
	mTickerCount = SDL_GetTicks();

	//Updating the game logic

	//Actor based code
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}

	mUpdatingActors = false;

	for (auto actor : mPendingActors)
	{
		mActors.emplace_back(actor);
	}

	mPendingActors.clear();

	//Add any dead actors to temp vector
	std::vector<Actor* > deadActors;

	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::State::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto it = deadActors.begin(); it != deadActors.end(); it++)
	{
		delete *it;
	}
}

void Game::AddActor(Actor* actor)
{
	//add actors to pending actors if all the current actors are updating
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}

	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{

	//Check if the vectors contain the actor
	//If they do, remove it from the vector
	auto it = std::find(mPendingActors.begin(), mPendingActors.end(), actor);

	if (it!=mPendingActors.end())
	{
		mPendingActors.erase(it);
	}

	it = std::find(mActors.begin(), mActors.end(), actor);

	if (it != mActors.end())
	{
		mActors.erase(it);
	}
}

SDL_Texture* Game::LoadTexture(std::string filename)
{
	//Load from file
	SDL_Surface* surface = IMG_Load(filename.c_str());

	if (!surface)
	{
		SDL_Log("Image could not be loaded");
		return nullptr;
	}

	//Create a texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
	SDL_FreeSurface(surface);
	if (!texture)
	{
		SDL_Log("Texture not made");
		return nullptr;
	}

	return texture;
}

SDL_Texture* Game::GetTexture(std::string file)
{
	auto it = textures.find(file);

	if (it != textures.end())
	{
		return textures[file];
	}

	else
	{
		SDL_Texture* temp = LoadTexture(file);
		textures.emplace(file, temp);
		return temp;
	}
}


void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); iter++)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (iter != mSprites.end())
	{
		mSprites.erase(iter);
	}
}