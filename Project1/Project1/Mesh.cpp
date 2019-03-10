#include"Game.h"
#include "Mesh.h"
#include<sstream>
#include<fstream>
#include<rapidjson/document.h>
#include<SDL.h>
#include"Math.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

bool Mesh::Load(std::string& filename, Game* game)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		SDL_Log("Mesh could not be loaded");
		return false;
	}

	std::stringstream stream;
	stream << file.rdbuf();
	//Reading the file in string format
	std::string contents = stream.str();

	//Converting the string to a json string
	const char* charContents = contents.c_str();
	rapidjson::StringStream jsonStr(charContents);

	//Parsing the string to a json file
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("The file was no a json object");
		return false;
	}

	int version = doc["version"].GetInt();

	if (version != 1)
	{
		SDL_Log("Version is not one" , filename.c_str());
		return false;
	}

	shaderName = doc["shader"].GetString();

	//Size of each vertex in vertex array
	int vertSize = 8;

	//Loading the textures array in the mesh
	const rapidjson::Value& texturesJson = doc["textures"];

	if (!texturesJson.IsArray() || texturesJson.Size() < 1)
	{
		SDL_Log("There were no textures in the mesh");
		return false;
	}

	//if the textures were loaded
	for (rapidjson::SizeType i = 0; i < texturesJson.Size(); i++)
	{
		std::string texName = texturesJson[i].GetString();
		//get the texture from the game
		Texture* t = game->GetTexture(texName);
		if (t == nullptr)
		{
			//Now try loading the texture again
			t = game->GetTexture(texName);
			if (t == nullptr)
			{
				t = game->GetTexture("Assets/Ship01.png");
			}
		}
		this->textures.emplace_back(t);
	}

	//Get the vertices from the json
	const rapidjson::Value& vertsJson = doc["vertices"];

	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh has no vertices", filename);
		return false;
	}

	std::vector<float> vertices;

	radius = 0.0;

    //each vertex is an array itself
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		//get the vertex array of each vertex
		const rapidjson::Value& verts = vertsJson[i];

		if (!verts.IsArray() || verts.Size() != 8)
		{
			SDL_Log("Unexpected vertex format for %s", filename);
			return false;
		}

		//the first three values of the vertex array are the position of the vertex
		Vector3 pos(verts[0].GetDouble(), verts[1].GetDouble(), verts[2].GetDouble());

		//Calculating the radius of the maximum bounding square
		//Comparing it with each vertex to find max radius
		radius = std::max(radius,pos.Length());

		//add the floats to the vertices
		for (rapidjson::SizeType i = 0; i < verts.Size(); i++)
		{
			vertices.emplace_back(static_cast<float>(verts[i].GetDouble()));
		}

	}

	//load the indices of the mesh
	const rapidjson::Value& indexJson = doc["indices"];

	if (!indexJson.IsArray() || indexJson.Size() < 1)
	{
		SDL_Log("The mesh has no indices", filename);
		return false;
	}

	std::vector<unsigned int> indices;

	for (rapidjson::SizeType i = 0; i < indexJson.Size(); i++)
	{
		const rapidjson::Value& inds = indexJson[i];
		if (!inds.IsArray() || inds.Size() != 3)
		{
			SDL_Log("Invalid index format", filename);
			return false;
		}

		indices.emplace_back(inds[0].GetUint());
		indices.emplace_back(inds[1].GetUint());
		indices.emplace_back(inds[2].GetUint());

	}

	//now create vertex array object using these values
	vertexArray = new VertexArray(vertices.data(),
		static_cast<float>(vertices.size()) / vertSize,
		indices.data(),
		static_cast<unsigned int>(indices.size()));	

	return true;

}
