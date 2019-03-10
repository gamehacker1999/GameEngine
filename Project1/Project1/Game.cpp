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

	//setting the open gl attributes

	//Select the core profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

	//Set up the version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);

	//Requesting a color buffer for 8 bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);

	//Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	//Force opengl to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);


	//creating sdl window
	mWindow = SDL_CreateWindow("Game Program", 100, 100, 1024, 768,SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(mWindow);

	glewExperimental = GLU_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize glew");
		return false;
	}
	glGetError();

	if (!mWindow)
	{
		SDL_Log("Window not created: &s", SDL_GetError());
		return false;
	}

	if (!LoadShaders())
	{
		SDL_Log("Shaders could not be loaded");
		return false;
	}

	InitSpriteVerts();


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
	temp->SetPosition(Vector3(0, 0,0));

	//Create a far background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024, 768));
	std::vector<Texture* >bgtexts = {
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
		i.second->Unload();
	}

	textures.clear();

}

void Game::Shutdown()
{
	SDL_GL_DeleteContext(context);
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
	//Set the clear color
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);

	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT);

	spriteShader->SetActive();
	spriteVerts->SetActive();

	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA
	);

	for (auto sprite : mSprites)
	{
		sprite->Draw(spriteShader);
	}
	//Swap the buffers
	SDL_GL_SwapWindow(mWindow);
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


Texture* Game::GetTexture(std::string file)
{
	auto it = textures.find(file);
	Texture* tex = nullptr;

	if (it != textures.end())
	{
		tex = it->second;
	}

	else
	{
		tex = new Texture();
		if (tex->Load(file))
		{
			textures.emplace(file, tex);
		}

		else
		{
			delete tex;
			tex = nullptr;
		}
	}

	return tex;
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

void Game::InitSpriteVerts()
{
	float vertices[] = {
	-0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
	 0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
	 0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
	-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	spriteVerts = new VertexArray(vertices, 4, indices, 6);
}

bool Game::LoadShaders()
{
	spriteShader = new Shader();
	if (!spriteShader->Load("Sprite.vert", "Sprite.frag"))
	{
		return false;
	}
	spriteShader->SetActive();
	Matrix4 viewProjection = Matrix4::CreateSimpleViewProj(1024, 769);
	spriteShader->SetMatrixUniform("uViewProj", viewProjection);
	return true;
}