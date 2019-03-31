#include "VertexArray.h"
#include<SDL.h>
#include<GL/glew.h>

VertexArray::VertexArray(const float * verts, unsigned int numVerts, const unsigned int * indices, unsigned int numIndices)
{

	this->numVerts = numVerts;
	this->numIndices = numIndices;

	//Creating vertex array object and storing its id in the vertex array member
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	//Creating vertex buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); //gl array corresponds to vertex buffer

	glBufferData(GL_ARRAY_BUFFER, //The recently made vertex buffer
		numVerts * 8 * sizeof(float), //Assume each vertex has 3 coordinates which are floats
		verts, //Source of the vertices
		GL_STATIC_DRAW); //Import the data once and draw it frequently

	//Creating index buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer); //gl element array corresponds to index buffer

	//copying the indices data to index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		numIndices * sizeof(unsigned int),
		indices,
		GL_STATIC_DRAW);

	//specifying the vertex layout, each vert has 3 coordinates associated with it

	//enabling the first vertex attribute
	glEnableVertexAttribArray(0);

	//Specifying the size, type and format for the attribute made above
	glVertexAttribPointer(
		0, //Attribute 0
		3, //number of components
		GL_FLOAT, //Type of components
		GL_FALSE, //only used for integral types
		sizeof(float) * 8, //Stride, size of each vertex
		0);// offset from start of vertex to this attribute

	//Attribute for normal coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 8,
		reinterpret_cast<void*>(sizeof(float) * 3)
	);

	//Texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
		reinterpret_cast<void*>(sizeof(float) * 6));
}

VertexArray::~VertexArray()
{
	//Deleting the buffers
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vertexArray);

}

void VertexArray::SetActive()
{
	glBindVertexArray(vertexArray);
}