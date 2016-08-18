/*
File:			Enemy.h
Description:	Manages enemy all existing enemy objects
Author:			Curtis Buckoll
*/

#pragma once

#include <vector>
#include <GLM/glm.hpp>
#include <random>

#include "SpriteMap.h"
#include "Texture2D.h"
#include "Weapon.h"
#include "EnemyShip.h"

class Enemy
{
private:
	std::vector<EnemyShip> _enemyArray;
	std::vector<Texture2D> _textureArray;
	Weapon _weapon;		///< Common weapon class manages all enemy projectiles.

public:

	std::mt19937 _rng; ///< for randomization

	Enemy();
	~Enemy();

	// Load textures
	void Init();

	/*	Getters for Collision checking	*/	

	glm::vec3 GetDirection(int index);
	glm::vec3 GetPosition(int index);
	glm::vec3 GetShotPosition(int index);
	float GetRadius(int index);
	float GetShotRadius(int index);
	int GetEnemyArraySize();
	int GetShotArraySize();

	/*	Enemy Operations  */

	void Add(ENEMY_TYPE type, const glm::vec3& player_pos);

	void Move();

	void Shoot(const glm::vec3& player_pos);

	// Draw ships and projectiles
	void Draw(GLuint shader_id);

	// Remove from given index
	void Remove(int index);

	void RemoveShot(int index);

	void DeleteAll();
};

