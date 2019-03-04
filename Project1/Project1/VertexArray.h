#pragma once
//Vertex array object class to score vertex buffer and index buffer
class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices,
	unsigned int numIndices);
	~VertexArray();

	//Activate the vertx array so we can draw it
	void SetActive();

	unsigned int GetNumIndices() const { return numIndices; }
	unsigned int GetNumVertices() const { return numVerts; }

private:
	//How many verts in vertex buffer
	unsigned int numVerts;
	//How many indices in vertex buffer
	unsigned int numIndices;
	//OpenGL id of the vertex buffer
	unsigned int vertexBuffer;
	//opengl id of the index buffer
	unsigned int indexBuffer;
	//opengl id of vertex array object
	unsigned int vertexArray;
};

