#pragma once
#include<string>
#include"Game.h"
#include"VertexArray.h"
#include"Renderer.h"
#include"Texture.h"
#include<vector>
class Mesh
{
public:
	Mesh();
	~Mesh();
	bool Load(std::string& filename, class Renderer* renderer);
	void Unload();
	//Get vertex array associated with this mesh
	VertexArray* GetVertexArray() const { return vertexArray; }
	//Get a texture from a specified index
	Texture* GetTexture(int index);
	//Get name of shader
	const std::string& GetShaderName() const { return shaderName; }
	//Get bounding sphere radius
	float GetRadius() const { return radius; }
	//Get specularPower
	float GetSpecularPower() const { return specularPower; }

private:
	std::vector<class Texture*> textures;
	VertexArray* vertexArray;
	//Name of the shader specified by mesh
	std::string shaderName;
	float radius;
	float specularPower;
	
};

