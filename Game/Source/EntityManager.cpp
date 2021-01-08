#include "EntityManager.h"

#include "App.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"

#include "GroundEnemy.h"
#include "Hearts.h"
#include "Coins.h"
#include "AirEnemy.h"
#include "CheckPoint.h"
#include "Boss.h"
#include "MovingPlatform.h"
#include "PlayerEntity.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

bool EntityManager::Awake()
{
	return true;
}

bool EntityManager::Start()
{
	gEnemyTexture = app->tex->Load("Assets/Textures/enemy.png");
	aEnemyTexture = app->tex->Load("Assets/Textures/enemy.png");
	movingPlatformTexture = app->tex->Load("Assets/Textures/moving_platform.png");
	heartsTexture = app->tex->Load("Assets/Textures/life.png");
	coinsTexture = app->tex->Load("Assets/Textures/coins.png");
	texCheckpoint = app->tex->Load("Assets/Textures/check_point.png");
	texBoss = app->tex->Load("Assets/Textures/boss.png");
	texPlayer = app->tex->Load("Assets/Textures/player.png");

	return true;
}

bool EntityManager::PreUpdate()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	ListItem<Entity*>* entity = entityList.start;

	while (entity != nullptr)
	{
		if (entity->data->pendingToDelete)
		{
			delete entity->data;
			entityList.Del(entity);
			entity = entity->next;
			continue;
		}

		entity->data->Update(dt);
		entity = entity->next;
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	if (app->scene1->active == true || app->scene2->active == true || app->scene3->active == true || app->scene4->active == true)
	{
		SDL_Rect rectPlayer;
		app->render->DrawTexture(heartsTexture, playerData.position.x, playerData.position.y, &rectPlayer);
		for (int i = 0; i < playerData.lives; i++)
		{
			app->render->DrawTexture(heartsTexture, ((-app->render->camera.x + 10) + (i * 32)) / 3, (-app->render->camera.y + 20) / 3, NULL);
		}
	}

	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->Draw();
	}

	playerData.hit = false;

	return true;
}

bool EntityManager::CleanUp()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->pendingToDelete = true;
	}

	entityList.Clear();

	return true;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	pugi::xml_node entities = data.child("entity");

	pugi::xml_node node = entities.child("player");
	pugi::xml_node node2 = entities.child("enemies");

	pugi::xml_node e;

	int count = 0;

	for (e = node.child("playerdata"); e; e = e.next_sibling("playerdata"))
	{
		float x = e.attribute("x").as_float();
		float y = e.attribute("y").as_float();
		fPoint newPosition = fPoint(x, y);
		Entity* entities = entityList[count];
		if (entities->type == Entity::Type::PLAYER)
		{
			entities->position = newPosition;
			count++;
		}
	}

	count = 1;

	for (e = node2.child("enemy"); e; e = e.next_sibling("enemy"))
	{
		float x = e.attribute("x").as_float();
		float y = e.attribute("y").as_float();
		fPoint newPosition = fPoint(x, y);
		Entity* enemies = entityList[count];
		if (enemies->type == Entity::Type::GROUND_ENEMY || enemies->type == Entity::Type::AIR_ENEMY)
		{
			enemies->position = newPosition;
			count++;
		}
	}

	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node entities = data.append_child("entity");
	pugi::xml_node node = entities.append_child("player");
	pugi::xml_node node2 = entities.append_child("enemies");

	for (int i = 0; i < entityList.Count(); i++)
	{
		Entity* e = entityList[i];
		if (e->type == Entity::Type::PLAYER)
		{
			pugi::xml_node eNode = node.append_child("playerdata");
			pugi::xml_attribute x = eNode.append_attribute("x");
			x.set_value(e->position.x);
			pugi::xml_attribute y = eNode.append_attribute("y");
			y.set_value(e->position.y);
			eNode.next_sibling("playerdata");
		}

		if (e->type == Entity::Type::GROUND_ENEMY || e->type == Entity::Type::AIR_ENEMY)
		{
			pugi::xml_node eNode = node2.append_child("enemy");
			pugi::xml_attribute x = eNode.append_attribute("x");
			x.set_value(e->position.x);
			pugi::xml_attribute y = eNode.append_attribute("y");
			y.set_value(e->position.y);
			eNode.next_sibling("enemy");
		}
	}

	return true;
}

void EntityManager::AddEntity(fPoint position, Entity::Type type)
{
	switch (type)
	{
	case Entity::Type::GROUND_ENEMY:
		gEnemy = (Entity*)(new GroundEnemy((Module*)this, position, gEnemyTexture, type));
		entityList.Add(gEnemy);
		break;
	case Entity::Type::HEARTS:
		heart = (Entity*)(new Hearts((Module*)this, position, heartsTexture, type));
		entityList.Add(heart);
		break;
	case Entity::Type::COINS:
		coin = (Entity*)(new Coins((Module*)this, position, coinsTexture, type));
		entityList.Add(coin);
		break;
	case Entity::Type::AIR_ENEMY:
		aEnemy = (Entity*)(new AirEnemy((Module*)this, position, aEnemyTexture, type));
		entityList.Add(aEnemy);
		break;
	case Entity::Type::CHECKPOINT:
		checkpoint = (Entity*)(new CheckPoint((Module*)this, position, texCheckpoint, type));
		entityList.Add(checkpoint);
		break;
	case Entity::Type::BOSS:
		boss = (Entity*)(new Boss((Module*)this, position, texBoss, type));
		entityList.Add(boss);
		break;
	case Entity::Type::MOVINGPLATFORM:
		movingPlatform = (Entity*)(new MovingPlatform((Module*)this, position, movingPlatformTexture, type));
		entityList.Add(movingPlatform);
		break;
	case Entity::Type::PLAYER:
		playerEntity = (Entity*)(new PlayerEntity((Module*)this, position, texPlayer, type));
		entityList.Add(playerEntity);
		break;
	}
	

}

void EntityManager::OnCollision(Collider* a, Collider* b)
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);

		if (entity->data->collider == a && b != nullptr)
		{
			entity->data->Collision(b);
		}

		if (entity->data->collider == b && a != nullptr)
		{
			entity->data->Collision(a);
		}
	}
}