#include "SpriteMap.h"
#include <iostream>


/* Modify to accept user and ship type as parameters, then to load the corresponding texture and attributes.
	Probably manage textures with another class containing array of SpriteMaps and all loaded textures on hand 
	Remove width and height parameters from constructor */

SpriteMap::SpriteMap()
{
	_width = 0.0f;
	_height = 0.0f;
	_worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	_type = SHIP_PLAYER;
	_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	_vao = 0;
	_vbo = 0;
	_moveSpeed = 1.0f;
}

SpriteMap::SpriteMap(Texture2D texture, float width, float height, glm::vec3 position, float moveSpeed, OBJECT_TYPE object)
{
	_width = width;
	_height = height;
	_worldPosition = position;
	_type = object;
	_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	_texture = texture;
	_moveSpeed = moveSpeed;

		/* INIT VERTEX DATA */

	switch (_type) {
		case SHIP_PLAYER :
			CreateShipData(_vertexData, _width, _height);
			break;
		case SHIP_ENEMY:
			CreateEnemyData(_vertexData, _width, _height);
			break;
		case BACKGROUND :
			CreateBackgroundData(_vertexData);
			break;
		default :
			CreateShotData(_vertexData, _width, _height);
			break;
	}

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer and attribute pointers
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, _vertexData, GL_DYNAMIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat) + 4 * sizeof(GLubyte), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color attribute exists but is unused here

	// Texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat) + 4 * sizeof(GLubyte), (GLvoid*)(3 * sizeof(GLfloat) + 4 * sizeof(GLubyte)));
	glEnableVertexAttribArray(1);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


SpriteMap::~SpriteMap()
{
	// Moved to function below. Call Manually.
}

void SpriteMap::Delete()
{
	if (_vao != 0) {
		glDeleteVertexArrays(1, &_vao);
	}
	if (_vbo != 0) {
		glDeleteBuffers(1, &_vbo);
	}
}

void SpriteMap::SetTexture(const Texture2D& texture)
{
	_texture = texture;
}

void SpriteMap::SetDirection(glm::vec3 direction)
{
	_direction = direction;
}

void SpriteMap::SetPosition(glm::vec3 position)
{
	_worldPosition = position;
}

void SpriteMap::UpdatePosition()
{
	_worldPosition += _direction * _moveSpeed;
}

// Primarily for player controls
void SpriteMap::UpdatePosition(glm::vec3 direction)
{
	_direction += direction * _moveSpeed;
	_worldPosition += _direction;
}

void SpriteMap::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture.GetTextureID());

	glBindVertexArray(_vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

// This function strictly affects only BACKGROUND textures
void SpriteMap::BackgroundScroll(float scroll_rate)
{
	if (_type != BACKGROUND) {
		std::cout << "ERROR: Cannot scroll non-background texture" << std::endl;
		return;
	}

	for (int i = 0; i < 6; i++) {
		_vertexData[i].texCoord.v += scroll_rate;
	}

	//Update data on GPU
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, _vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

glm::vec3 SpriteMap::GetPosition()
{
	return _worldPosition;
}

glm::vec3 SpriteMap::GetDirection()
{
	return _direction;
}

