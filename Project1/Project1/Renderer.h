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
#include"MeshComponent.h"
#include"DirectionalLight.h"
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

	void AddMesh(class MeshComponent* mesh);

	void RemoveMesh(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& filename);

	class Mesh* GetMesh(const std::string& filename);

	void SetAmbientLight(const Vector3& ambient) {ambientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return directionLight; }

private:
	bool LoadShaders();
	void InitSpriteVerts();

	std::map<std::string, Texture* > textures;
	std::vector<class SpriteComponent*> sprites;

	std::map<std::string, class Mesh*> meshes;
	std::vector<class MeshComponent*> meshComponents;

	class Game* game;

	//Sprite shader
	class Shader* spriteShader;
	//Mesh shader
	class Shader* meshShader;

	//Matrices for view and projection
	Matrix4 view;
	Matrix4 projection;
	//sprite vertex array
	class VertexArray* spriteVerts;

	SDL_Window* window;

	SDL_GLContext context;

	//fields for lighting
	Vector3 ambientLight;
	DirectionalLight directionLight;
	void SetLightUniforms(Shader* shader);
};

