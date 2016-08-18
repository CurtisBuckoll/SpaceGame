#include "Shot.h"
#include <iostream>


Shot::Shot()
{
}

Shot::Shot(glm::vec3 position, float width, float height, float speed, int lifetime, Texture2D texture)
{
	_position = position;
	_lifetime = lifetime;
	_shotSpeed = speed;
	_sprite = SpriteMap(texture, width, height, position, 1.0f, SHOT);
	_radius = width / 3;			
}

Shot::~Shot()
{
	// Use Delete() as manual destructor
}

void Shot::Delete()
{
	_sprite.Delete();
}

void Shot::Move()
{
	if (_lifetime <= 0)
	{
		Delete();
		return;
	}

	_position += glm::vec3(0.0f, 1.0f, 0.0f) * _shotSpeed;

	_lifetime--;

}

void Shot::Draw()
{
	if (_lifetime <= 0)
	{
		Delete();
		return;
	}
	_sprite.Draw();
}

glm::vec3 Shot::GetPosition()
{
	return _position;
}

float Shot::GetRadius()
{
	return _radius;
}

float Shot::GetLifetime()
{
	return _lifetime;
}