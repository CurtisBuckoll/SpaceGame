
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <vector>
#include "Texture2D.h"
#include "SpriteMap.h"

class ExplAnim
{
private:

	SpriteMap _sprite;

public:

	int lifetime; ///In frames
	int currentTexture;

	ExplAnim();
	~ExplAnim();

	ExplAnim(glm::vec3 position, glm::vec3 direction, Texture2D texture);

	void Draw();

	void ChangeTexture(Texture2D texture);

	void Delete();

	glm::vec3 GetPosition();

};

class Explosion
{
private:


public:
	std::vector<ExplAnim> _explosion;
	std::vector<Texture2D> _textures;
	GLuint _shader_id;

	Explosion();
	~Explosion();

	// Load textures
	void Init(GLuint shader_id);

	void Add(glm::vec3 position, glm::vec3 direction);

	// Draws all at their current frame state
	void Draw();

	void DeleteAt(int index);

	void DeleteAll();
};

