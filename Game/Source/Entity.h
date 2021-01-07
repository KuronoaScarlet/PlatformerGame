#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Module.h"
#include "Animation.h"
#include "Textures.h"
#include "Point.h"


class Entity
{
public:
	enum class Type
	{
		UNKNOWN,
		GROUND_ENEMY,
		AIR_ENEMY,
		HEARTS,
		COINS,
		CHECKPOINT,
		BOSS,
		MOVINGPLATFORM
	};

	Entity(Module* listener, fPoint position, SDL_Texture* texture, Type type) : listener(listener), position(position), texture(texture), type(type)
	{}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	virtual void Collision(Collider* coll)
	{

	}

	virtual void CleanUp()
	{

	}

	Module* listener;

	Type type;
	fPoint position;
	SDL_Texture* texture;

	Collider* collider = nullptr;

	bool pendingToDelete = false;
};

#endif // __ENTITY_H__