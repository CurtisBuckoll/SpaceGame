#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <string>
#include <vector>

#include "SpriteMap.h"
#include "Texture2D.h"
#include "Shot.h"

// USER - for declaring owner
enum USER {
	PLAYER = 0,
	ENEMY,
};

// WEAPON_TYPE - for indexing textures and specifying type
enum WEAPON_TYPE {
	GREEN_PROJ = 0,
	BLUE_PROJ,
	ROCKET ///etc
	/// ^ add more to this later on probably if want to add more player/enemy weapon types.
};

class Weapon
{
private:

	USER _user;
	std::vector<Texture2D> _textures;
	std::vector<Shot> _shots;

public:

	Weapon();
	~Weapon();

	// Load shot textures to memory
	void Init(USER user);

	void DeleteAll();

	// Append to _shot vector
	void Add(glm::vec3 pos, WEAPON_TYPE type);

	// Remove from an index
	void Remove(int index);

	// Draws to screen, then updates changes in projectile position
	void Draw(GLuint shader_id);

	glm::vec3 GetPosition(int index);

	float GetRadius(int index);

	int GetArrSize()
	{
		return _shots.size();
	}
};
