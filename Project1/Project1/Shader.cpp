#include "Shader.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<SDL.h>
Shader::Shader()
{
}


Shader::~Shader()
{
	vertexShader = 0;
	fragmentShader = 0;
	shaderProgram = 0;
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	//open file
	std::fstream shaderFile(fileName);

	if (shaderFile.is_open())
	{
		//storing the entire shader in a single string
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		//Create a shader of the specific type
		outShader = glCreateShader(shaderType);
		//Set the source characters and try to compile
		glShaderSource(outShader, 1, &contentsChar, nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("could not compile shader");
			return false;
		}

	}

	else
	{
		SDL_Log("could not find shader file");
		return false;
	}

	return true;
}

//check to see if shader compiled
bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	//Query the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL did not compile:\n%s", buffer);
		return false;
	}
	return true;
}

//Compile both shaders and combine them
bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	if(!CompileShader(vertName,GL_VERTEX_SHADER,vertexShader)||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, fragmentShader))
	{
		return false;
	}

	//Linking the vertex and frag shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, vertexShader);
	glLinkProgram(shaderProgram);

	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}

//See if the program is valid
bool Shader::IsValidProgram()
{
	GLint status;
	//Query the compile status
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[100000];
		memset(buffer,0, 100000);
		glGetProgramInfoLog(shaderProgram, 100000, nullptr,buffer);
		SDL_Log("Program is not valid:\n %s", buffer);
		return false;
	}
	return true;
}

void Shader::SetActive()
{
	glUseProgram(shaderProgram);
}

void Shader::Unload()
{
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::SetMatrixUniforms(const char* name, const Matrix4* matrix, int size)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);

	glUniformMatrix4fv(loc, size, GL_TRUE, matrix->GetAsFloatPtr());
}


void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	//find the unform by this name
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	//set matrix data to uniform
	glUniformMatrix4fv(
		loc, //Uniform ID
		1, //Number of matrices
		GL_TRUE, //True if using row matrix
		matrix.GetAsFloatPtr() //Pointer to matrix
	);
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	//finding the uniform in the shader
	GLuint loc = glGetUniformLocation(shaderProgram, name);

	//setting the vector data
	glUniform3fv(loc, 1,vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, const float& floatNum)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	glUniform1f(loc, floatNum);
}

