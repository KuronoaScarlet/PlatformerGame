#ifndef _GROUNDENEMY_H_
#define _GROUNDENEMY_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class GroundEnemy : Entity
{
public:
	GroundEnemy(Module* listener, fPoint position, SDL_Texture* texture, Type type);
	
	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* coll);
	
	void CleanUp();

private:
	Animation idleAnimation;
	Animation walkAnimRight;

	Animation* currentAnimation;

	uint hitFx = 0;
};

#endif // _GROUNDENEMY_H_