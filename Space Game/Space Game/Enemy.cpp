#include "Enemy.h"

#include <time.h>
#include <functional>

#include <iostream>
#include <GL/glew.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


Enemy::Enemy() 
{
	// Empty - std::vector constructor initializes _enemyArray and _textureArray for us.
}

Enemy::~Enemy()
{
	// Empty - Use manual Delete();
}

void Enemy::Init()
{
	_rng = std::mt19937(time(0));
	_weapon.Init(ENEMY);

	/* Load shot textures to memory, matching indices of ENEMY_TYPE: */

	// Index 0 - EN_0
	_textureArray.emplace_back("../deps/textures/Gepard.png", PNG_RGBA);
	// Index 1 - EN_1
	_textureArray.emplace_back("../deps/textures/stateczek.png", PNG_RGBA);
}

glm::vec3 Enemy::GetDirection(int index)
{
	return _enemyArray[index].GetDirection();
}

glm::vec3 Enemy::GetPosition(int index)
{
	return _enemyArray[index].GetPosition();
}

glm::vec3 Enemy::GetShotPosition(int index)
{
	return _weapon.GetPosition(index);
}

float Enemy::GetRadius(int index)
{
	return _enemyArray[index].GetRadius();
}

float Enemy::GetShotRadius(int index)
{
	return _weapon.GetRadius(index);
}

int Enemy::GetEnemyArraySize()
{
	return _enemyArray.size();
}

int Enemy::GetShotArraySize()
{
	return _weapon.GetArrSize();
}

void Enemy::Add(ENEMY_TYPE type, const glm::vec3& player_pos)
{
	_enemyArray.emplace_back(type, _textureArray[type], player_pos, &_rng);
}

void Enemy::Move()
{
	for (int i = 0; i < _enemyArray.size(); i++)
	{
		_enemyArray[i].Move();
	}
}

// Causes all enemy ships meeting requirements to fire
// Approximation amount specified in here: x_DIST
void Enemy::Shoot(const glm::vec3& player_pos)
{
	float x_DIST = 0.5;

	for (int i = 0; i < _enemyArray.size(); i++)
	{
		if (_enemyArray[i].CanShoot() && abs(_enemyArray[i].GetPosition().x - player_pos.x) <= x_DIST )
		{
			_weapon.Add(_enemyArray[i].GetPosition() + glm::vec3(0.0f, -0.12f, 0.0f), GREEN_PROJ);
		}
	}
}

// Draw ships and projectiles
void Enemy::Draw(GLuint shader_id)
{
	for (int i = 0; i < _enemyArray.size(); i++)
	{
		_enemyArray[i].Draw(shader_id);
	}

	_weapon.Draw(shader_id);
}

// Remove from given index
void Enemy::Remove(int index)
{
	/// need to test this probably!
	_enemyArray[index].Delete();
	_enemyArray[index] = _enemyArray.back();
	_enemyArray.pop_back();
}

void Enemy::RemoveShot(int index)
{
	_weapon.Remove(index);
}

void Enemy::DeleteAll()
{
	for (int i = 0; i < _enemyArray.size(); i++)
	{
		_enemyArray[i].Delete();
	}
	for (int i = 0; i < _textureArray.size(); i++)
	{
		_textureArray[i].Delete();
	}
	_weapon.DeleteAll();
}