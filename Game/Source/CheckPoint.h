#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Module.h"
#include "Animation.h"
#include "Entity.h"

struct SDL_Texture;
struct Collider;

class CheckPoint : Entity
{
public:
	// Constructor
	CheckPoint(Module* listener, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	//void Collision(Collider* coll);

	void CleanUp();
	
	bool on = false;
	
private:

	// A set of animations
	Animation idleAnimation;
	Animation activatedAnimation;
	Animation* currentAnimation = nullptr;

};

#endif //!__MODULE_PLAYER_H__
