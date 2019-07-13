#include"Game.h"
#include"Renderer.h"
#include "Mesh.h"
#include<string>
#include<sstream>
#include<fstream>
#include<rapidjson/document.h>
#include<SDL.h>
#include"Math.h"
#include"VertexArray.h"
#include"MeshComponent.h"


union Vertex
{
	float f;
	uint8_t b[4];
};


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

bool Mesh::Load(std::string& fileName, Renderer* renderer,std::string texture)
{
	bool success = false;

	if (fileName.find(".obj") != std::string::npos)
	{
		success = LoadOBJ(fileName, renderer, texture);
	}

	else if (fileName.find(".gpmesh") != std::string::npos)
	{
		success = LoadGPMESH(fileName, renderer, texture);
	}

	else if (fileName.find(".fbx") != std::string::npos)
	{
		success = LoadFBX(fileName, renderer, texture);
	}

	return success;

}

void Mesh::Unload()
{
	delete vertexArray;
	vertexArray = nullptr;
}

Texture * Mesh::GetTexture(int index)
{
	if (index < textures.size())
	{
		return textures[index];
	}

	else
	{
		return nullptr;
	}
}

bool Mesh::LoadFBX(std::string & fileName, Renderer * renderer, std::string texture)
{
	return false;
}

bool Mesh::LoadOBJ(std::string & fileName, Renderer * renderer, std::string texture)
{
	std::ifstream file;

	file.open(fileName.c_str(), std::ios::in);

	std::string line;

	if (file.is_open())
	{
		//vector to hold the vertices, normals, indices, and texture coordinates
		std::vector<Vector3> tempVertices;
		std::vector<float> tempIndices;
		std::vector<Vector3> tempNormals;
		std::vector<Vector2> tempTextureCoord;

		//vectors to hold the final position of the vertices indices, normals, and textures
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<float> normals;
		std::vector<float> textureCoord;

#pragma region getting the vertex, texture, and normal coordinates of the model

		while (std::getline(file, line))
		{
			std::vector<std::string> words;

			int pos = 0;
			int curPos = 0;
			while (pos <= line.length())
			{
				pos = line.find(" ", curPos);
				std::string word = line.substr(curPos, pos - curPos);
				curPos = pos + 1;
				words.emplace_back(word);
			}

			if (line.find("v ") == 0)
			{
				tempVertices.emplace_back(Vector3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
			}

			else if (line.find("vn ") == 0)
			{
				tempNormals.emplace_back(Vector3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
			}

			else if (line.find("vt ") == 0)
			{
				tempTextureCoord.emplace_back(Vector2(std::stof(words[1]), std::stof(words[2])));
			}

			else if (line.find("f ") == 0)
			{
				vertices.resize(8 * tempVertices.size());
				std::fill(vertices.begin(), vertices.begin() + vertices.capacity(), -1);
				break;
			}

		}

#pragma endregion

#pragma region Getting the index array of the model

		while (line.length() > 0)
		{
			if (!(line.find("f ") == 0))
			{
				std::getline(file, line);
				continue;
			}

			std::vector<std::string> words; //this hold all the vertices

			//splitting the normal array
			int pos = 0;
			int curPos = 0;
			while (pos <= line.length())
			{
				(pos = line.find(" ", curPos));
				std::string word = line.substr(curPos, pos - curPos);
				curPos = pos + 1;
				words.emplace_back(word);
			}

			std::vector<std::vector<std::string>> listOfFaces;

			for (int i = 0; i < words.size()-1; i++)
			{
				std::vector<std::string> vertex;

				pos = 0;
				curPos = 0;

				while (pos <= words[i+1].length())
				{
					(pos = words[i+1].find("/", curPos));
					std::string word = words[i+1].substr(curPos, pos - curPos);
					curPos = pos + 1;
					vertex.emplace_back(word);
				}

				listOfFaces.emplace_back(vertex);
			}

			std::string firstVert = listOfFaces[0][0];
			for (int i = 1; i < listOfFaces.size()-1; i++)
			{
				indices.emplace_back(std::stoi(firstVert)-1);
				indices.emplace_back(std::stoi(listOfFaces[i][0])-1);
				indices.emplace_back(std::stoi(listOfFaces[i+1][0])-1);
			}

			for (int i = 0; i < listOfFaces.size(); i++)
			{
				int location = 8 * (std::stoi(listOfFaces[i][0]) - 1);

				vertices[location] = tempVertices[std::stoi(listOfFaces[i][0]) - 1].x;
				vertices[location + 1] = tempVertices[std::stoi(listOfFaces[i][0]) - 1].y;
				vertices[location + 2] = tempVertices[std::stoi(listOfFaces[i][0]) - 1].z;

				//the three coordinates of the normal position
				vertices[location + 3] = tempNormals[std::stoi(listOfFaces[i][2]) - 1].x;
				vertices[location + 4] = tempNormals[std::stoi(listOfFaces[i][2]) - 1].y;
				vertices[location + 5] = tempNormals[std::stoi(listOfFaces[i][2]) - 1].z;

				//the three coordinates of the texture coordinates
				vertices[location + 6] = tempTextureCoord[std::stoi(listOfFaces[i][1]) - 1].vx;
				vertices[location + 7] = 1 - tempTextureCoord[std::stoi(listOfFaces[i][1]) - 1].vy;
			}

#pragma endregion

			std::getline(file, line);
		}

		Texture* t = renderer->GetTexture(texture);
		if (t == nullptr)
		{
			// Try loading the texture
			t = renderer->GetTexture(texture);
			if (t == nullptr)
			{
				// If it's still null, just use the default texture
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		this->textures.emplace_back(t);

#pragma endregion

		vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / 8
			, indices.data(), indices.size(), VertexArray::Layout::PosNormTex);
		return true;
	}

	return false;
}

bool Mesh::LoadGPMESH(std::string & fileName, Renderer * renderer, std::string texture)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Mesh %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the version
	if (ver != 1)
	{
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	shaderName = doc["shader"].GetString();

	// Set the vertex layout/size based on the format in the file
	VertexArray::Layout layout = VertexArray::PosNormTex;
	size_t vertSize = 8;

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkinTex")
	{
		layout = VertexArray::PosNormSkinTex;
		// This is the number of "Vertex" unions, which is 8 + 2 (for skinning)s
		vertSize = 10;
	}

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}

	specularPower = static_cast<float>(doc["specularPower"].GetDouble());

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// Is this texture already loaded?
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// Try loading the texture
			t = renderer->GetTexture(texName);
			if (t == nullptr)
			{
				// If it's still null, just use the default texture
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		this->textures.emplace_back(t);
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	radius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// For now, just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		radius = Math::Max(radius, pos.LengthSq());
		//mBox.UpdateMinMax(pos);

		if (layout == VertexArray::PosNormTex)
		{
			Vertex v;
			// Add the floats
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		else
		{
			Vertex v;
			// Add pos/normal
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// Add skin information
			for (rapidjson::SizeType j = 6; j < 14; j += 4)
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// Add tex coords
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = vert[j].GetDouble();
				vertices.emplace_back(v);
			}
		}
	}

	// We were computing length squared earlier
	radius = Math::Sqrt(radius);

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array
	vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		indices.data(), indices.size(), layout);
	return true;
}

