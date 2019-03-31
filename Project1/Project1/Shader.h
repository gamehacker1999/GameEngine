#pragma once
#include<string>
#include<SDL.h>
#include<GL/glew.h>
#include"Math.h"
class Shader
{
public:
	Shader();
	~Shader();
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	//set this as active shader program
	void SetActive();
	//Add matrix transforms to the shader
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetVectorUniform(const char* name, const Vector3& vector3);
	void SetFloatUniform(const char* name, const float& floatNum);

private:
	//tries to compile the specific shader
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	//Test weather shader compiled successfully
	bool IsCompiled(GLuint shader);
	//Text weather glsl programs link
	bool IsValidProgram();

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
};

