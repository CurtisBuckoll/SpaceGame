#include "Collision.h"

#include <GLM/glm.hpp>
#include "Explosion.h"

extern Explosion extern_Explosion;

void Collision::EnemyHit(Player* player, Enemy* enemy)
{
	float collision_dist;
	float actual_dist;

	for (int i = 0; i < player->GetShotArraySize(); i++)
	{
		for (int k = 0; k < enemy->GetEnemyArraySize(); k++)
		{
			collision_dist = player->GetShotRadius(i) + enemy->GetRadius(k);
			actual_dist = glm::distance(player->GetShotPosition(i), enemy->GetPosition(k));
			if (actual_dist <= collision_dist)
			{
				extern_Explosion.Add(player->GetShotPosition(i),  glm::normalize(enemy->GetDirection(k)) + glm::vec3(0.0f, 1.0f, 0.0f) );
				player->RemoveShot(i);
				enemy->Remove(k);
				break;
			}
		}
	}
}

void Collision::PlayerHit(Player* player, Enemy* enemy)
{
	float collision_dist;
	float actual_dist;

	for (int i = 0; i < enemy->GetShotArraySize(); i++)
	{
		collision_dist = enemy->GetShotRadius(i) + player->GetRadius();
		actual_dist = glm::distance(enemy->GetShotPosition(i), player->GetPosition());
		if (actual_dist <= collision_dist)
		{
			extern_Explosion.Add(enemy->GetShotPosition(i), glm::vec3(0.0f, -1.0f, 0.0f));
			enemy->RemoveShot(i);
			player->DecrementHP();
			//player->Delete();
			///Do something with player
			break;
		}
	}
}

void Collision::ShipsCollide(Player* player, Enemy* enemy)
{
	float collision_dist;
	float actual_dist;

	for (int i = 0; i < enemy->GetEnemyArraySize(); i++)
	{
		collision_dist = player->GetRadius() + enemy->GetRadius(i);
		actual_dist = glm::distance(player->GetPosition(), enemy->GetPosition(i));
		if (actual_dist <= collision_dist)
		{
			extern_Explosion.Add(player->GetPosition(), player->GetDirection());
			enemy->Remove(i);
			///Do something with player
			break;
		}
	}
}
