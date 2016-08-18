/*
File:			EnemyShip.h
Description:	Improves upon SpriteBatch to define and control instantiation and behavior (AI) for a given Enemyship object.
Author:			Curtis Buckoll
*/

#pragma once

#include <GLM/glm.hpp>
#include <vector>
#include <random>

#include "SpriteMap.h"
#include "Texture2D.h"
#include "Weapon.h"

/* The following entries of EnemyType define the variations of enemies */
enum ENEMY_TYPE {
	EN_0,
	EN_1,
	EN_2
};

class EnemyShip
{
private:

	SpriteMap _sprite;
	ENEMY_TYPE _enemyType;
	WEAPON_TYPE _weaponType;
	float _radius;
	float _creationTime;

	// Define frames/shot
	int _fireRate;
	int _cooldown;

	// Private methods
	glm::vec3 GenStartPos(std::mt19937* rng);
	glm::vec3 GenStartDir(const glm::vec3& player_pos, std::mt19937* rng);

public:

	EnemyShip();
	EnemyShip(ENEMY_TYPE type, Texture2D texture, const glm::vec3& player_pos, std::mt19937* rng);
	~EnemyShip();

	// Move to appropriate frame position
	void Move();

	// Return true if cooldown over, update cooldown counter otherwise. Call this every frame to maintain correct timing.
	bool CanShoot();

	glm::vec3 GetPosition();

	glm::vec3 GetDirection();

	float GetRadius();

	void Draw(GLuint shader_id);

	// Free memory
	void Delete();
};

