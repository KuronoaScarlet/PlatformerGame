#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class CheckPoint : public Module
{
public:
	// Constructor
	CheckPoint();

	// Destructor
	~CheckPoint();

	bool Awake();

	// Called when the module is activated
	// Loads the necessary textures for the CheckPoint
	bool Start() override;

	// Called at the middle of the application loop
	
	bool PreUpdate();

	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of the CheckPoint sprite
	bool PostUpdate();

	bool CleanUp();

	void InitialPos();
	
	bool on = false;
	
private:

	// The speed in which we move the player (pixels per frame)
	int speed = 1;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;

	
	
	// A set of animations
	Animation idleAnim;
	Animation activated;
	Animation* currentAnim = nullptr;


	fPoint position;

	
	// The Checkpoint's collider
	Collider* collider = nullptr;

};

#endif //!__MODULE_PLAYER_H__
