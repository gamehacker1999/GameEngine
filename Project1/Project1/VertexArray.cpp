#include "VertexArray.h"
#include<SDL.h>
#include<GL/glew.h>

VertexArray::VertexArray(const void * verts, unsigned int numVerts, const unsigned int * indices, unsigned int numIndices, Layout layout)
{

	this->numVerts = numVerts;
	this->numIndices = numIndices;

	unsigned vertexSize = 8 * sizeof(float);

	if (layout == PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float)+8*sizeof(char);
	}

	//Creating vertex array object and storing its id in the vertex array member
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	//Creating vertex buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); //gl array corresponds to vertex buffer

	glBufferData(GL_ARRAY_BUFFER, //The recently made vertex buffer
		numVerts * vertexSize, //Assume each vertex has 3 coordinates which are floats
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

	if (layout == PosNormTex)
	{
		//enabling the first vertex attribute
		glEnableVertexAttribArray(0);

		//Specifying the size, type and format for the attribute made above
		glVertexAttribPointer(
			0, //Attribute 0
			3, //number of components
			GL_FLOAT, //Type of components
			GL_FALSE, //only used for integral types
			vertexSize, //Stride, size of each vertex
			0);// offset from start of vertex to this attribute

		//Attribute for normal coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3)
		);

		//Texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}

	else if (layout == PosNormSkinTex)
	{
		// Position is 3 floats
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// Normal is 3 floats
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// Skinning indices (keep as ints)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));

		// Skinning weights (convert to floats)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));

		// Texture coordinates
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));


	}
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