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
		HEARTS,
		COINS
	};

	Entity(fPoint position, SDL_Texture* texture, Type type) : position(position), texture(texture), type(type)
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

	Type type;
	fPoint position;
	SDL_Texture* texture;

	bool pendingToDelete = false;
};

#endif // __ENTITY_H__