#pragma once
#include<GL/glew.h>
#include<vector>
#include<string>
#include<SOIL.h>
#include"Texture.h"
#include"VertexArray.h"
#include"Shader.h"
class Skybox
{
public:
	Skybox();
	~Skybox();
	unsigned int LoadCubeMap(std::vector<std::string> textures);
	void Draw();
	bool GenerateVertexArray();

private:
	//VertexArray* vertexArray;
	//Texture* t;
	//Shader* shader;
	GLuint skyboxVAO;
	GLuint skyboxVBO;
	GLuint texturesID;
};

