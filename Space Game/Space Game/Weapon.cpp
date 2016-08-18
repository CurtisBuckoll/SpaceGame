#include "Weapon.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


Weapon::Weapon()
{

}

Weapon::~Weapon()
{
	// Use manual destructor
}

void Weapon::Init(USER user)
{	
	_user = user;
	
	// Load shot textures to memory, matching indices of WEAPON_TYPE
	_textures.emplace_back("../deps/textures/spr_bullet_green.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/spr_bullet_blue.png", PNG_RGBA);
}

void Weapon::DeleteAll()
{
	for (int i = 0; i < _shots.size(); i++)
	{
		_shots[i].Delete();
	}
	for (int i = 0; i < _textures.size(); i++)
	{
		_textures[i].Delete();
	}
}

void Weapon::Add(glm::vec3 pos, WEAPON_TYPE type) 
{
	float speed, width, height;
	int lifetime, vert_dir;

	(_user == PLAYER) ? vert_dir = 1 : vert_dir = -1;

	switch (type)
	{
	case GREEN_PROJ :
		speed = 0.022;
		lifetime = 200;
		width = height = 0.08;
		break;

	case BLUE_PROJ :
		speed = 0.03;
		lifetime = 200;
		width = 0.10;
		height = 0.40;
		break;
	}

	_shots.emplace_back(pos, width, height, vert_dir * speed, lifetime, _textures[type]);
}

void Weapon::Remove(int index)
{
	_shots[index].Delete();
	_shots[index] = _shots.back();
	_shots.pop_back();
}

void Weapon::Draw(GLuint shader_id)
{
	GLuint modelLoc = glGetUniformLocation(shader_id, "model");

	for (int i = 0; i < _shots.size(); i++)
	{
		/* DRAW */

		glm::mat4 shotModel;
		shotModel = glm::translate(shotModel, _shots[i].GetPosition());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(shotModel));

		_shots[i].Draw();


		/* UPDATE PROJECTILE POSITION */

		if (_shots[i].GetLifetime() <= 0)
		{
			_shots.erase(_shots.begin() + i);			///<CAN DO THIS MORE EFFECIENTLY.. swap with back, pop back?
		}
		else
		{
			_shots[i].Move();
		}
	}
}

glm::vec3 Weapon::GetPosition(int index)
{
	return _shots[index].GetPosition();
}

float Weapon::GetRadius(int index)
{
	return _shots[index].GetRadius();
}