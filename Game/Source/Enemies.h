#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "Module.h"
#include "Animation.h"

#define grav = -0.1f

struct SDL_Texture;
struct Collider;
struct Enemy
{
public:
	fPoint position;
	SString name;
	float posx, posy;
	float vely = 0.0f;
	float velx = 0.0f;
	SDL_Texture* texture;
	Animation* currentAnim = nullptr;
};


class Enemies : public Module
{
public:
	Enemies();

	// Destructor
	virtual ~Enemies();

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

	bool CheckCollision(int x, int y);
	void OnCollision(Collider* a, Collider* b);
	void InitialPos();

	bool enemyJumping = true;
	float MAX_FALL_SPEED = 1.0f;
	float gravity = 0.2f;
	float delta = 1.5f;
	bool godMode = false;
	bool doubleJump = false;
	Enemy enemyd;


private:

	SDL_Texture* character;
	Animation walkAnimRight;
	Animation walkAnimLeft;
	Animation jumpAnim;
	Animation idleAnim;
	Collider* collider;
	bool onGround = false;
	bool isDead;
	bool debug = false;
	bool cameraControl = true;
};

#endif // __MODULE_ENEMIES_H__