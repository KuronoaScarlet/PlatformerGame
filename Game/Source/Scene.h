#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

#define grav = -0.1f

struct SDL_Texture;
struct Player;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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

	struct Player
	{
	public:
		SString name;
		float posx, posy;
		float vely = 0.0f;
		SDL_Texture* texture;
	};

private:
	Player* player = new Player;
	SDL_Texture* character;
};


#endif // __SCENE_H__