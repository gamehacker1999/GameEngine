#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Game.h"
#include <GL/glew.h>
#include "SkeletalMeshComponent.h"


Renderer::Renderer(class Game* game)
{
	this->game = game;
}


Renderer::~Renderer()
{
	/*delete meshShader;
	delete spriteShader;
	delete skinnedMeshShader;
	delete spriteVerts;
	*/
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

	//enable depth buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//Force opengl to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


	//creating sdl window
	window = SDL_CreateWindow("Game Program", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);

	if (!window)
	{
		SDL_Log("Window not created: &s", SDL_GetError());
		return false;
	}

	context = SDL_GL_CreateContext(window);

	glewExperimental = GLU_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize glew");
		return false;
	}
	glGetError();

	if (!LoadShaders())
	{
		SDL_Log("Shaders could not be loaded");
		return false;
	}

	InitSpriteVerts();
	return true;
}

void Renderer::Shutdown()
{
	UnloadData();
	delete spriteVerts;
	spriteShader->Unload();
	delete spriteShader;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
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
	//Set the clear color
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//drawing the meshes
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	meshShader->SetActive();
	meshShader->SetMatrixUniform("uViewProj", view*projection);

	SetLightUniforms(meshShader);
	for (auto mesh : meshComponents)
	{
		mesh->Draw(meshShader);
	}

	//drawing the skinned meshes
	skinnedMeshShader->SetActive();
	skinnedMeshShader->SetMatrixUniform("uViewProj", view*projection);
	SetLightUniforms(skinnedMeshShader);

	for (auto mesh : skeletalMeshComponent)
	{
		mesh->Draw(skinnedMeshShader);
	}
	

	//drawing the sprites
	spriteShader->SetActive();
	//spriteVerts->SetActive();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA
	);

	for (auto sprite : sprites)
	{
		sprite->Draw(spriteShader);
	}


	//Swap the buffers
	SDL_GL_SwapWindow(window);
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

Texture * Renderer::GetTexture(const std::string& file)
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

Mesh * Renderer::GetMesh(const std::string & filename)
{
	auto it = meshes.find(filename);
	Mesh* mesh = nullptr;

	if (it != meshes.end())
	{
		mesh = it->second;
	}

	else
	{
		mesh = new Mesh();
		if (mesh->Load((std::string&)filename,this))
		{
			meshes.emplace(filename, mesh);
		}

		else
		{
			delete mesh;
			mesh = nullptr;
		}
	}

	return mesh;
}

bool Renderer::LoadShaders()
{
	//Initializing sprite shader
	spriteShader = new Shader();
	if (!spriteShader->Load("Sprite.vert", "Sprite.frag"))
	{
		return false;
	}
	spriteShader->SetActive();
	Matrix4 viewProjection = Matrix4::CreateSimpleViewProj(1024, 768);
	spriteShader->SetMatrixUniform("uViewProj", viewProjection);

	//Initializing mesh shader
	meshShader = new Shader();
	if (!meshShader->Load("Phong.vert", "Phong.frag"))
	{
		return false;
	}

	skinnedMeshShader = new Shader();

	if (!skinnedMeshShader->Load("Skinned.vert", "Phong.frag"))
	{
		return false;
	}

	meshShader->SetActive();
	//skinnedMeshShader->SetActive();

	//Creating a view matrix
	view = Matrix4::CreateLookAt(
		Vector3::Zero, //eye
		Vector3::UnitX, //Target
		Vector3::UnitZ //Up
	);

	//creating projection matrix
	projection = Matrix4::CreatePerspectiveFOV(
		Math::ToRadians(70), //Field of view
		1024, //height
		768, //width
		25, //near plane
		10000// far plane
	);

	meshShader->SetMatrixUniform("uViewProj", view*projection);
	skinnedMeshShader->SetMatrixUniform("uViewProj", view*projection);

	return true;
}

void Renderer::InitSpriteVerts()
{
	float vertices[] = {
	-0.5f,  0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, // top left
	 0.5f,  0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, // top right
	0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, // bottom right
	-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
	};
	spriteVerts = new VertexArray(vertices, 4, indices, 6,VertexArray::PosNormTex);
}

void Renderer::SetLightUniforms(Shader * shader)
{
	//camera position is the inverse of the view matrix
	Matrix4 inView = view;
	inView.Invert();
	//getting the world space position of camera
	shader->SetVectorUniform("uCameraPos", inView.GetTranslation());
	//setting the ambient light
	shader->SetVectorUniform("ambientLight", ambientLight);
	//setting the directional light
	shader->SetVectorUniform("directionalLight.direction",directionLight.direction);
	shader->SetVectorUniform("directionalLight.diffuseColor",directionLight.diffuseColor);
	shader->SetVectorUniform("directionalLight.specularity",directionLight.speculariy);

}

void Renderer::AddMesh(MeshComponent* mesh)
{
	if (mesh->isSkeletal)
	{
		skeletalMeshComponent.emplace_back((class SkeletalMeshComponent*)mesh);
		return;
	}
	meshComponents.emplace_back(mesh);
}

void Renderer::RemoveMesh(MeshComponent* mesh)
{
	if (mesh->isSkeletal)
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(skeletalMeshComponent.begin(), skeletalMeshComponent.end(), sk);
		skeletalMeshComponent.erase(iter);
		return;
	}

	meshComponents.erase(std::remove(meshComponents.begin(), meshComponents.end(), mesh), meshComponents.end());
}
