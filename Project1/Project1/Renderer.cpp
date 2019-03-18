#include "Renderer.h"



Renderer::Renderer(class Game* game)
{
	this->game = game;
}


Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Set up the version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//Requesting a color buffer for 8 bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	//Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Force opengl to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


	//creating sdl window
	window = SDL_CreateWindow("Game Program", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);

	glewExperimental = GLU_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize glew");
		return false;
	}
	glGetError();

	if (!window)
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
}

void Renderer::Shutdown()
{
	UnloadData();
	delete spriteVerts;
	spriteShader->Unload();
	delete spriteShader;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	//SDL_DestroyRenderer(mRenderer);
}

void Renderer::UnloadData()
{
	for (auto i : textures)
	{
		i.second->Unload();
		delete i.second;
	}
	textures.clear();
}

void Renderer::Draw()
{
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = sprites.begin();
	for (; iter != sprites.end(); iter++)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	sprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite), sprites.end());
}

Texture * Renderer::GetTexture(std::string & filename)
{
	return nullptr;
}

Mesh * Renderer::GetMesh(std::string & filename)
{
	return nullptr;
}

bool Renderer::LoadShaders()
{
	return false;
}

void Renderer::InitSpriteVerts()
{
}
