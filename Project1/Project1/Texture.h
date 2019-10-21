#pragma once
//Class to add texture to opengl
#include<string>
class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& filename);
	void Unload();

	void SetActive();

	int GetWidth() const { return width; }
	int GetHeight() const{ return height; }

private:
	//opengl id of this texture
	unsigned int textureID;
	int width;
	int height;
};

