/*
File:			Collision.h
Description:	Methods for checking collision and performing the correct response.
Author:			Curtis Buckoll
*/

#pragma once

#include "Player.h"
#include "Enemy.h"

static class Collision
{
public:

	static void EnemyHit(Player* player, Enemy* enemy);

	static void PlayerHit(Player* player, Enemy* enemy);

	static void ShipsCollide(Player* player, Enemy* enemy);

};

