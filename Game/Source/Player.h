#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"

#define grav = -0.1f

struct SDL_Texture;
struct Playerd
{
public:
	SString name;
	float posx, posy;
	float vely = 0.0f;
	float velx = 0.0f;
	SDL_Texture* texture;
	Animation* currentanim;
};


class Player : public Module
{
public:

	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//For saving and loading player position
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool playerjumping = true;
	float MAX_FALL_SPEED = 1.0f;
	float gravity = 0.2f;
	float delta = 1.5f;
	bool godmode = false;
	bool doublejump = false;

	Playerd Playerd;
	

private:

	SDL_Texture* character;
	Animation* walkAnim = new Animation();
	Animation idleAnim;
};


#endif // __SCENE_H__
