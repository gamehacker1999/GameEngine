#include "MeshComponent.h"

MeshComponent::MeshComponent(class Actor* owner):Component(owner)
{
	mOwner->GetGame()->GetRenderer()->AddMesh(this);
	//meshIndex = 0;
}


MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMesh(this);
}

void MeshComponent::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void MeshComponent::Draw(Shader* shader)
{
	if (mesh)
	{
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
		Texture* t = mesh->GetTexture(meshIndex);
		if (t)
			t->SetActive();

		VertexArray* va = mesh->GetVertexArray();
		va->SetActive();

		shader->SetFloatUniform("specularPower", mesh->GetSpecularPower());

		//draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
