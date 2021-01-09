#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Entity.h"

#include "GuiButton.h"

class EntityManager : public Module
{
public:
	EntityManager();

	// Destructor
	virtual ~EntityManager()
	{}

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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void AddEntity(fPoint position, Entity::Type type);

	void OnCollision(Collider* a, Collider* b);

	struct PlayerData 
	{
		fPoint position;
		int lives = 3;

		bool hit = false;
		bool godMode = false;

		fPoint initPositionScene1 = { 50.0f, 260.0f };
		fPoint initPositionScene2 = { 50.0f, 278.0f };
		fPoint initPositionScene3 = { 32.0f, 277.0f };
		fPoint initPositionScene4 = { 32.0f, 277.0f };

		//Pause Menu
		bool pauseCondition = false;
		bool debug = false;
		bool resetCamera;

		SDL_Texture* pauseMenu = nullptr;

		GuiButton* resumeButton;
		GuiButton* settingsButton;
		GuiButton* backToTitleButton;
		GuiButton* exitButton;

		SDL_Rect cameraBckUp;
	};

	PlayerData playerData;

private:
	List<Entity*> entityList;

	SDL_Texture* gEnemyTexture;
	SDL_Texture* aEnemyTexture;
	SDL_Texture* heartsTexture;
	SDL_Texture* coinsTexture;
	SDL_Texture* texCheckpoint;
	SDL_Texture* texBoss;
	SDL_Texture* movingPlatformTexture;
	SDL_Texture* texPlayer;


	Entity* gEnemy;
	Entity* aEnemy;
	Entity* heart;
	Entity* coin;
	Entity* checkpoint;
	Entity* boss;
	Entity* movingPlatform;
	Entity* playerEntity;
};

#endif // _ENTITYMANAGER_H_