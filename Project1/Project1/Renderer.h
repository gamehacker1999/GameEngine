#pragma once

#include <string>
#include <vector>
#include <map>
#include <SDL.h>
#include "Math.h"
#include"DirectionalLight.h"
#include"SpriteComponent.h"
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

	class Mesh* GetMesh(const std::string& filename,std::string texture="default");

	void SetAmbientLight(const Vector3& ambient) {ambientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return directionLight; }

	void SetViewMatrix(Matrix4 view) { this->view = view; }

private:
	bool LoadShaders();
	void InitSpriteVerts();

	std::map<std::string, Texture* > textures;
	std::vector<class SpriteComponent*> sprites;

	std::map<std::string, class Mesh*> meshes;
	std::vector<class MeshComponent*> meshComponents;
	std::vector<class SkeletalMeshComponent*> skeletalMeshComponent;

	class Game* game;

	//Sprite shader
	class Shader* spriteShader;
	//Mesh shader
	class Shader* meshShader;
	//skinned mesh shader
	class Shader* skinnedMeshShader;

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

