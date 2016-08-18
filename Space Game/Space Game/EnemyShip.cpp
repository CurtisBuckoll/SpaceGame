#include "EnemyShip.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

glm::vec3 EnemyShip::GenStartPos(std::mt19937* rng)
{
	float x, y, z = 0.0f;
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	switch (_enemyType) 
	{
	case EN_0 :
		x = (dist(*rng) * 2.0f) - 1.0f;
		y = 1.2f;
		z = 0.0f;
		break;

	case EN_1 :
		/// Add implementation later
		((dist(*rng) * 2.0f) - 1.0f) < 0 ? x = -1.2 : x = 1.2;
		y = 0.6f + (dist(*rng) * 0.3f);
		z = 0.0f;
		break;

	case EN_2 :
		/// Add implementation later
		break;
	}

	return glm::vec3(x, y, z);
}

glm::vec3 EnemyShip::GenStartDir(const glm::vec3& player_pos, std::mt19937* rng)
{
	glm::vec3 direction;
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	switch (_enemyType)
	{
	case EN_0:
		direction = player_pos - _sprite.GetPosition();
		if (direction.x > 0.0f)
		{
			direction.x += dist(*rng); //or just += 0.05f maybe
		}
		else
		{
			direction.x -= dist(*rng);
		}
		break;

	case EN_1:
		if (_sprite.GetPosition().x < -1.0f)
		{
			direction.x = 1.0f; //or just += 0.05f maybe
		}
		else
		{
			direction.x = -1.0f;
		}
		break;
	}

	glm::normalize(direction);
	return direction;
}

EnemyShip::EnemyShip()
{
	//Empty - use parameterized constructor only!
	_fireRate = 0;
	_cooldown = 0;
}

//Define ship characteristics here in constructor
EnemyShip::EnemyShip(ENEMY_TYPE enemyType, Texture2D texture, const glm::vec3& player_pos, std::mt19937* rng)
{
	_enemyType = enemyType;
	_cooldown = 150;
	_creationTime = glfwGetTime();
	
	switch (enemyType) 
	{
	case EN_0 :
		_weaponType = GREEN_PROJ;
		_fireRate = 170;
		_sprite = SpriteMap(texture, 0.22, 0.27, GenStartPos(rng), 0.002, SHIP_ENEMY);
		_radius = 0.05;
		break;

	case EN_1:
		_weaponType = GREEN_PROJ;
		_fireRate = 130;
		_sprite = SpriteMap(texture, 0.23, 0.29, GenStartPos(rng), 0.0015, SHIP_ENEMY);
		_radius = 0.07;
		/// Add implementation later
		break;

	case EN_2:
		/// Add implementation later
		break;
	}

	_sprite.SetDirection(GenStartDir(player_pos, rng));
}

EnemyShip::~EnemyShip()
{
	//Empty - use Delete();
}

void EnemyShip::Move()
{
	glm::vec3 newDirec;

	switch (_enemyType)
	{
	case EN_0 :
		_sprite.UpdatePosition();
		break;

	case EN_1:
		newDirec = _sprite.GetDirection();
		newDirec.y = 3.0f * sin((glfwGetTime() + _creationTime) * 2.0f);
		if (_sprite.GetPosition().x <= -1.0f)
		{
			newDirec.x = 1.0f;
		}
		if (_sprite.GetPosition().x >= 1.0f)
		{
			newDirec.x = -1.0f;
		}
		_sprite.SetDirection(newDirec);
		_sprite.UpdatePosition();
		/// Add implementation later
		break;

	case EN_2:
		/// Add implementation later
		break;
	}
}

bool EnemyShip::CanShoot()
{
	if (_cooldown <= 0)
	{
		_cooldown = _fireRate;
		return true;
	}
	else
	{
		_cooldown--;
		return false;
	}
}

glm::vec3 EnemyShip::GetPosition()
{
	return _sprite.GetPosition();
}

glm::vec3 EnemyShip::GetDirection()
{
	return _sprite.GetDirection();
}

float EnemyShip::GetRadius()
{
	return _radius;
}

void EnemyShip::Draw(GLuint shader_id)
{
	GLuint modelLoc = glGetUniformLocation(shader_id, "model");

	glm::mat4 modelEnemy;
	modelEnemy = glm::translate(modelEnemy, _sprite.GetPosition());
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEnemy));

	_sprite.Draw();
}

void EnemyShip::Delete()
{
	_sprite.Delete();
}