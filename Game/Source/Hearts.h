#ifndef _HEARTS_H_
#define _HEARTS_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Hearts : Entity
{
public:
	Hearts(fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

private:
	Animation idleAnimation;
	Animation* currentAnimation;
};

#endif // _HEARTS_H_