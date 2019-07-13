#include "Texture.h"
#include<SDL.h>
#include<GL/glew.h>
#include<SOIL.h>
#include<iostream>

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
}


Texture::~Texture()
{
}

bool Texture::Load(const std::string& filename)
{
	//Store number of channels
	int channels = 0;
	//load the image
	//Store the width height and channel data in the class object
	unsigned char* image = SOIL_load_image(
		filename.c_str(),
		&width,
		&height,
		&channels,
		SOIL_LOAD_AUTO //Type of image file
	);

	

	if (image == nullptr)
	{
		SDL_Log("Failed to load image, %s",filename.c_str());
		return false;
	}

	//Determine if image is rgba or rgb
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	//generate texture object and set it as active
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Copy the raw image data to glTexImage2D
	glTexImage2D(
		GL_TEXTURE_2D, //Texture object
		0, //Level of detail
		format,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE, //Format of the color channels
		image
	);
	//freeing image data from memory
	SOIL_free_image_data(image);

	//enable bilinear filtering
	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &textureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}