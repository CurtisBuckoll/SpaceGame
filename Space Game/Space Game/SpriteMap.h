/*
File:			SpriteMap.h
Description:	Class for game objects: player and computer controlled ships/prjectiles.
				Manages associated textures, world size, world coordinates, and drawing (if needed).
Author:			Curtis Buckoll
*/

#pragma once

#include <GL/glew.h>
#include <string>
#include <GLM/glm.hpp>

#include "Texture2D.h"
#include "Vertex.h"


//Must initialize player controlled ship to index 0
enum OBJECT_TYPE {
	SHIP_PLAYER,
	SHIP_ENEMY,
	BACKGROUND,
	SHOT,
	OTHER
};


class SpriteMap
{
private:

	Texture2D _texture;
	Vertex _vertexData[6];
	glm::vec3 _worldPosition;
	glm::vec3 _direction;
	float _moveSpeed;
	GLuint _vao;
	GLuint _vbo;

	OBJECT_TYPE _type;
	float _width;
	float _height;

public:

	SpriteMap();

	// Paramaterized constructor/destructor
	SpriteMap(Texture2D texture, float width, float height, glm::vec3 position, float moveSpeed, OBJECT_TYPE object);
	~SpriteMap();

	// Manual destructor
	void Delete();

	// Change applied texture - does NOT delete previous texture
	void SetTexture(const Texture2D& texure);

	// Set private _direction variable
	void SetDirection(glm::vec3 direction);

	// Hard set position;
	void SetPosition(glm::vec3 position);

	// Update world position of game object
	void UpdatePosition();
	// (Overloaded version - manual direction - use to move player)
	void UpdatePosition(glm::vec3 direction);

	// Draw Object. World coordinates handled by vertexShader
	void Draw();

	// Update texture coordinates for scrolling background
	void BackgroundScroll(float scroll_rate);

	// Return position vector of object
	glm::vec3 GetPosition();

	// Return direction vector of object
	glm::vec3 GetDirection();
};

