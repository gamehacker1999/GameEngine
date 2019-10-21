#pragma once
#include "Component.h"
#include"Shader.h"
#include"Actor.h"
#include"Mesh.h"

//class to load 3d meshes
class MeshComponent :
	public Component
{
public:
	MeshComponent(class Actor* owner, bool isSkeleton = false);
	virtual ~MeshComponent();

	//draw this mesh component with shader
	virtual void Draw(class Shader* shader);

	//set the mesh and texture
	virtual void SetMesh(class Mesh* mesh);
	void SetTextureIndex(int index) { meshIndex = index; }

	bool isSkeletal;

protected:
	class Mesh* mesh;
	int meshIndex;
};

