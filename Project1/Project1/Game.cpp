#pragma once

#include "Game.h"
#include<Box2D.h>
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include <math.h>
#include<memory>
#include<SDL.h>
#include<SDL_image.h>
#include<GL/glew.h>
#include"VertexArray.h"
#include"PlaneActor.h"
#include"FPSActor.h"


Game::Game()
{
	mTickerCount = 0;
}


Skeleton * Game::GetSkeleton(const std::string & fileName)
{
	auto iter = mSkeletons.find(fileName);

	if (iter != mSkeletons.end())
	{
		return iter->second;
	}

	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(fileName))
		{
			mSkeletons.emplace(fileName, sk);
		}

		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

Animation * Game::GetAnimation(const std::string & fileName)
{
	auto iter = mAnimations.find(fileName);

	if (iter != mAnimations.end())
	{
		return iter->second;
	}

	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName))
		{
			mAnimations.emplace(fileName, anim);
		}

		else
		{
			delete anim;
			anim = nullptr;
		}

		return anim;
	}
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
	
	//Setting up the renderer
	renderer = new Renderer(this);
	renderer->Initialize();
	LoadData();


	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);

	mIsRunning = true;
	mTickerCount = SDL_GetTicks();
	return true;
}

void Game::LoadData()
{
	//create a ship
	/*mShip = new Ship(this);

	//Create actor for background
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector3(0, 0,0));

	//Create a far background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024, 768));
	std::vector<Texture* >bgtexts = {
		renderer->GetTexture("Assets/Farback01.png"),
		renderer->GetTexture("Assets/Farback02.png"),

	};

	bg->SetBGTextures(bgtexts);
	bg->SetScrollSpeed(-100);

	//Create a near background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024, 768));
	bgtexts = {
		renderer->GetTexture("Assets/Stars.png"),
		renderer->GetTexture("Assets/Stars.png")
	};	
	bg->SetBGTextures(bgtexts);
	bg->SetScrollSpeed(-200);
	*/

	Actor* a = new Actor(this);
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitZ, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(renderer->GetMesh("Assets/Cube.gpmesh"));

	a = new Actor(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(renderer->GetMesh("Assets/Sphere.gpmesh"));

	 //Setup floor
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// Left/right walls
	/*q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// Forward/back walls
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}*/

	//creating actors

	/*Actor* a = nullptr;
	Quaternion q;

	// Setup floor
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}*/

	followActor = new FollowActor(this);

	// Setup lights
	renderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = renderer->GetDirectionalLight();
	dir.direction = Vector3(0.0f, -0.707f, -0.707f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.speculariy = Vector3(0.8f, 0.8f, 0.8f);

	//FPS Camera actor
	//cameraActor = new CameraActor(this);
	//fpsActor = new FPSActor(this);
	//car = new Car(this);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	renderer->UnloadData();

}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	renderer->Shutdown();
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

	//mShip->ProcessKeyboard(state);
}

void Game::GenerateOutput()
{
	renderer->Draw();
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
		//compute world transform before adding into the game
		actor->CreateWorldTransform();
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
