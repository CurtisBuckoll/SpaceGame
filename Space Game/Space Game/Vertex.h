/*
File:			Vertex.h
Description:	Contains useful data structs for vertex position, colour, and texture coordinates
				Also contains 'constructor' like functions for initializing UV texture coordinates
Author:			Curtis Buckoll
*/


#pragma once

#include <GL/glew.h>

struct Position {
	float x;
	float y;
	float z;
};

struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct TexCoord {
	float u;
	float v;
};

struct Vertex {

	Position position;
	Color color;
	TexCoord texCoord;

	inline void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	inline void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
	inline void SetTexCoord(GLfloat u, GLfloat v);

};



// Struct methods for initializing local vertex cooordinates for standard size objects

inline void Vertex::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

inline void Vertex::SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

inline void Vertex::SetTexCoord(GLfloat u, GLfloat v)
{
	texCoord.u = u;
	texCoord.v = v;
}

// MUST take a size 36 Vertex array
inline void CreateCube(Vertex* arr)
{
	GLfloat data[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	}; //Size 180
	int index = 0;

	for (int i = 0; i < 36; i++)
	{
		arr[i].position.x = data[index++];
		arr[i].position.y = data[index++];
		arr[i].position.z = data[index++];

		arr[i].texCoord.u = data[index++];
		arr[i].texCoord.v = data[index++];

		arr[i].color.r = 0;
		arr[i].color.g = 0;
		arr[i].color.b = 0;
		arr[i].color.a = 0;
	}

}

//MUST take a size 6 Vertex array
inline void CreateBackgroundData(Vertex* data)
{
	data[0].SetPosition(-1.0f, -1.0f, 0.0f);
	data[0].SetTexCoord(0.0f, 0.0f);

	data[1].SetPosition(1.0f, 1.0f, 0.0f);
	data[1].SetTexCoord(1.0f, 1.0f);

	data[2].SetPosition(1.0f, -1.0f, 0.0f);
	data[2].SetTexCoord(1.0f, 0.0f);

	data[3].SetPosition(-1.0f, -1.0f, 0.0f);
	data[3].SetTexCoord(0.0f, 0.0f);

	data[4].SetPosition(1.0f, 1.0f, 0.0f);
	data[4].SetTexCoord(1.0f, 1.0f);

	data[5].SetPosition(-1.0f, 1.0f, 0.0f);
	data[5].SetTexCoord(0.0f, 1.0f);
}

//MUST take a size 6 Vertex array
inline void CreateShipData(Vertex* data, float width, float height)
{
	data[0].SetPosition(-width / 2, -height / 2, -1.0f);
	data[0].SetTexCoord(0.0f, 0.0f);

	data[1].SetPosition(width / 2, height / 2, -1.0f);
	data[1].SetTexCoord(1.0f, 1.0f);

	data[2].SetPosition(width / 2, -height / 2, -1.0f);
	data[2].SetTexCoord(1.0f, 0.0f);

	data[3].SetPosition(-width / 2, -height / 2, -1.0f);
	data[3].SetTexCoord(0.0f, 0.0f);

	data[4].SetPosition(width / 2, height / 2, -1.0f);
	data[4].SetTexCoord(1.0f, 1.0f);

	data[5].SetPosition(-width / 2, height / 2, -1.0f);
	data[5].SetTexCoord(0.0f, 1.0f);
}

inline void CreateEnemyData(Vertex* data, float width, float height)
{
	/* Perhaps to avoid depth collision problem, create a random z coordinate with a few decimal points of precision
	between -0.9000 and -0.9999 (or around there) for each new texture created */


	// **y coords flip texture orientation:

	data[0].SetPosition(-width / 2, height / 2, -0.9);
	data[0].SetTexCoord(0.0f, 0.0f);

	data[1].SetPosition(width / 2, -height / 2, -0.9);
	data[1].SetTexCoord(1.0f, 1.0f);

	data[2].SetPosition(width / 2, height / 2, -0.9);
	data[2].SetTexCoord(1.0f, 0.0f);

	data[3].SetPosition(-width / 2, height / 2, -0.9);
	data[3].SetTexCoord(0.0f, 0.0f);

	data[4].SetPosition(width / 2, -height / 2, -0.9);
	data[4].SetTexCoord(1.0f, 1.0f);

	data[5].SetPosition(-width / 2, -height / 2, -0.9);
	data[5].SetTexCoord(0.0f, 1.0f);
}

inline void CreateShotData(Vertex* data, float width, float height)
{

	data[0].SetPosition(-width / 2, -height / 2, -0.5);
	data[0].SetTexCoord(0.0f, 0.0f);

	data[1].SetPosition(width / 2, height / 2, -0.5);
	data[1].SetTexCoord(1.0f, 1.0f);

	data[2].SetPosition(width / 2, -height / 2, -0.5);
	data[2].SetTexCoord(1.0f, 0.0f);

	data[3].SetPosition(-width / 2, -height / 2, -0.5);
	data[3].SetTexCoord(0.0f, 0.0f);

	data[4].SetPosition(width / 2, height / 2, -0.5);
	data[4].SetTexCoord(1.0f, 1.0f);

	data[5].SetPosition(-width / 2, height / 2, -0.5);
	data[5].SetTexCoord(0.0f, 1.0f);
}