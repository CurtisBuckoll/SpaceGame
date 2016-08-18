/*
File:			Player.h
Description:	Manages player sprite creation, keyboard movement, firing, drawing
Author:			Curtis Buckoll
*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <vector>

#include "SpriteMap.h"
#include "Texture2D.h"
#include "Weapon.h"

enum PLAYER_STATUS {
	ALIVE = 0,
	DEAD
};

class Player
{
private:

	SpriteMap _player;
	Texture2D _texture;
	Weapon _weapon;
	WEAPON_TYPE _currentWeapon;
	float _radius;

	PLAYER_STATUS _status;
	SpriteMap _playerLife;
	int _lives;
	int _maxHP;
	int _hitpoints;

public:

	Player();
	~Player();

	// Initialize and load player texture
	void Init(float moveSpeed);

	/*	Getters for Collision checking	*/

	glm::vec3 GetDirection();
	glm::vec3 GetPosition();
	glm::vec3 GetShotPosition(int index);
	float GetRadius();
	float GetShotRadius(int index);
	int GetShotArraySize();

	// Draw to screen
	void Draw(GLuint shader_id);

	// Move according to keyboard input
	void Move(bool* keys);

	// Change the current weapon
	void ChangeWeapon(WEAPON_TYPE type);

	void AddShots(bool* keys);

	void RemoveShot(int index);

	// Manages hitpoints/lives and status
	void DecrementHP();

	int GetLives();

	// Free Resources
	void Delete();


};

