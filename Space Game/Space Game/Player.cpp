#include "Player.h"

#include <iostream>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <random>
#include "Explosion.h"
#include "Collision.h"


/* Globals for randomization/explosion creation */
std::mt19937 rng;
std::uniform_real_distribution<double> dist(0.0, 1.0);

extern Explosion extern_Explosion;


Player::Player()
{
}


Player::~Player()
{
}

// Initialize and load player texture
void Player::Init(float moveSpeed)
{
	_texture = Texture2D("../deps/textures/ship0.png", PNG_RGBA);
	_player = SpriteMap(_texture, 0.22f, 0.22f, glm::vec3(0.0f, -0.75f, 0.0f), moveSpeed, SHIP_PLAYER);
	_radius = 0.06;
	_weapon.Init(PLAYER);
	
	_status = ALIVE;
	_playerLife = SpriteMap(_texture, 0.07f, 0.07f, glm::vec3(0.9f, -0.9f, 0.0f), 0.0f, SHIP_PLAYER);
	_lives = 5;
	_maxHP = 10;
	_hitpoints = _maxHP;
}

glm::vec3 Player::GetDirection()
{
	return _player.GetDirection();
}

glm::vec3 Player::GetPosition()
{
	return _player.GetPosition();
}

glm::vec3 Player::GetShotPosition(int index)
{
	return _weapon.GetPosition(index);
}

float Player::GetRadius()
{
	return _radius;
}

float Player::GetShotRadius(int index)
{
	return _weapon.GetRadius(index);
}

int Player::GetShotArraySize()
{
	return _weapon.GetArrSize();
}

// Draw player, shots, lives to screen.
void Player::Draw(GLuint shader_id)
{
	GLuint modelLoc = glGetUniformLocation(shader_id, "model");

	glm::mat4 modelPlayer;
	modelPlayer = glm::translate(modelPlayer, _player.GetPosition());
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPlayer));

	_player.Draw();
	_weapon.Draw(shader_id);

	for (int i = 0; i < _lives; i++)
	{
		glm::mat4 modelLife;
		modelLife = glm::translate(modelLife, _playerLife.GetPosition() + glm::vec3(0.0f, i * 0.10f, 0.0f) );
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLife));
		_playerLife.Draw();
	}
		

	static int deathFrames = 200;
	if (_status == DEAD)
	{
		if (deathFrames <= 0) {
			_status = ALIVE;
			_player.SetPosition(glm::vec3(0.0f, -0.75f, 0.0f));
			_lives--;
			_hitpoints = _maxHP;
			deathFrames = 200;
		} else {
			if (deathFrames % 20 == 0)
			{
				extern_Explosion.Add(_player.GetPosition() + glm::vec3(dist(rng)*0.15, dist(rng)*0.15f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			}
			deathFrames--;
		}
	}

}

// Move according to keyboard input
void Player::Move(bool* keys)
{
	glm::vec3 input_direction = glm::vec3(0.0f, 0.0f, 0.0f);

	if (keys[GLFW_KEY_W] && _status == ALIVE) {
		input_direction += glm::vec3(0.0, 1.0, 0.0);
	}
	else if (_player.GetDirection().y > 0) {
		input_direction -= glm::vec3(0.0, 1.0, 0.0) / 2.0f;
	}

	if (keys[GLFW_KEY_S] && _status == ALIVE) {
		input_direction -= glm::vec3(0.0, 1.0, 0.0);
	}
	else if (_player.GetDirection().y < 0) {
		input_direction += glm::vec3(0.0, 1.0, 0.0) / 2.0f;
	}

	if (keys[GLFW_KEY_A] && _status == ALIVE) {
		input_direction -= glm::vec3(1.0, 0.0, 0.0);
	}
	else if (_player.GetDirection().x < 0) {
		input_direction += glm::vec3(1.0, 0.0, 0.0) / 2.0f;
	}

	if (keys[GLFW_KEY_D] && _status == ALIVE) {
		input_direction += glm::vec3(1.0, 0.0, 0.0);
	}
	else if (_player.GetDirection().x > 0) {
		input_direction -= glm::vec3(1.0, 0.0, 0.0) / 2.0f;
	}

	_player.UpdatePosition(input_direction);

	// Check for weapon change
	if (keys[GLFW_KEY_1]) {
		_currentWeapon = GREEN_PROJ;
	}
	if (keys[GLFW_KEY_2]) {
		_currentWeapon = BLUE_PROJ;
	}

	/* BOUNDS CHECKING */

	glm::vec3 worldPos = _player.GetPosition();
	glm::vec3 currDirec = _player.GetDirection();

	bool update = false;
	if (worldPos.x < -1.0f) {
		worldPos.x = 1.0f;
		update = true;
	}
	if (worldPos.x > 1.0f) {
		worldPos.x = -1.0f;
		update = true;
	}
	if (worldPos.y <= -1.0f) {
		worldPos.y = -0.9999f;
		currDirec.y = 0.0f;
		update = true;
	}
	if (worldPos.y >= 1.0f) {
		worldPos.y = 0.9999f;
		currDirec.y = 0.0f;
		update = true;
	}

	if (update) {
		_player.SetDirection(currDirec);
		_player.SetPosition(worldPos);
	}
}

void Player::ChangeWeapon(WEAPON_TYPE type)
{
	_currentWeapon = type;
}

void Player::AddShots(bool* keys)
{
	if (_status == DEAD)
		return;

	static bool fire;
	if ((keys[GLFW_KEY_SPACE] || keys[GLFW_KEY_UP]) && fire == true)
	{
		_weapon.Add(_player.GetPosition() + glm::vec3(0.0f, 0.12f, 0.0f), _currentWeapon);
		fire = false;

	}
	if (!keys[GLFW_KEY_SPACE] && !keys[GLFW_KEY_UP])
	{
		fire = true;
	}
}

void Player::RemoveShot(int index)
{
	_weapon.Remove(index);
}

void Player::DecrementHP()
{
	if (_status == ALIVE)
	{
		_hitpoints--;

		if (_hitpoints <= 0) {
			_status = DEAD;
		}
	}
}

int Player::GetLives()
{
	return _lives;
}

void Player::Delete()
{
	_player.Delete();
	_texture.Delete();
	_weapon.DeleteAll();
}