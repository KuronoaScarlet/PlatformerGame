#ifndef _COINS_H_
#define _COINS_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Coins : Entity
{
public:
	Coins(Module* listener, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* coll);

	void CleanUp();

private:
	Animation idleAnimation;

	Animation* currentAnimation;
};

#endif // _COINS_H_