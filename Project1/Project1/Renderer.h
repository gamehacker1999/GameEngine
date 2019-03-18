#pragma once
#include<SDL.h>
#include<string>
#include"Texture.h"
#include"Mesh.h"
#include<map>
#include"SpriteComponent.h"
#include"Game.h"
#include"Shader.h"
#include"GL/glew.h"
#include"VertexArray.h"
class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();
	bool Initialize();
	void Shutdown();

	void UnloadData();

	void Draw();

	void AddSprite(SpriteComponent* sprite);

	void RemoveSprite(SpriteComponent* sprite);

	class Texture* GetTexture(std::string& filename);

	class Mesh* GetMesh(std::string& filename);

private:
	bool LoadShaders();
	void InitSpriteVerts();

	std::map<std::string, Texture* > textures;

	std::vector<class SpriteComponent*> sprites;

	class Game* game;

	//Sprite shader
	class Shader* spriteShader;
	//sprite vertex array
	class VertexArray* spriteVerts;

	SDL_Window* window;

	SDL_GLContext context;
};

