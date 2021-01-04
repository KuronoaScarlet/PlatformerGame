#include "EntityManager.h"

#include "App.h"
#include "Log.h"
#include "Textures.h"

#include "GroundEnemy.h"
#include "Hearts.h"
#include "Coins.h"
#include "AirEnemy.h"
#include "CheckPoint.h"

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
	heartsTexture = app->tex->Load("Assets/Textures/life.png");
	coinsTexture = app->tex->Load("Assets/Textures/coins.png");
	texCheckpoint = app->tex->Load("Assets/Textures/check_point.png");

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
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->Draw();
	}

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
	pugi::xml_node enemies = data.child("enemies");

	pugi::xml_node e;

	int count = 0;

	for (e = enemies.child("enemy"); e; e = e.next_sibling("enemy"))
	{
		float x = e.attribute("x").as_float();
		float y = e.attribute("y").as_float();
		fPoint newPosition = fPoint(x, y);
		Entity* enemies = entityList[count];
		if (enemies->type == Entity::Type::GROUND_ENEMY || enemies->type == Entity::Type::AIR_ENEMY)
		{
			enemies->position = newPosition;
		}

		count++;
	}

	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node enemies = data.child("enemies");

	for (int i = 0; i < entityList.Count(); i++)
	{
		Entity* e = entityList[i];
		if (e->type == Entity::Type::GROUND_ENEMY || e->type == Entity::Type::AIR_ENEMY)
		{
			pugi::xml_node eNode = enemies.append_child("enemy");
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