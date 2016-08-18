/*
File:			Shot.h
Description:	Global shot class managing properties of all projectiles fired by enemy/player ships
Author:			Curtis Buckoll
*/

/*Place both enums in weapon. remove _weapon parameter from this class*/

#pragma once

#include <GLM/glm.hpp>
#include <string>

#include "SpriteMap.h"
#include "Texture2D.h"

/*
enum USER {
	PLAYER = 0,
	ENEMY,
};

enum WEAPON_TYPE {
	STANDARD = 0,
	LASER,
	ROCKET ///etc
		   /// ^ add more to this later on probably if want to add more player/enemy weapon types.
};
*/

class Shot
{
private:

	SpriteMap _sprite;
	float _shotSpeed;
	int _lifetime;
	glm::vec3 _position;
	float _radius;

public:
	// Constructors/Destructors
	Shot();
	Shot(glm::vec3 position, float width, float height, float speed, int lifetime, Texture2D texture);
	~Shot();

	void Delete();

	// Move according to shot speed
	void Move();

	// Draw to screen
	void Draw();

	glm::vec3 GetPosition();

	float GetRadius();

	float GetLifetime();

};

