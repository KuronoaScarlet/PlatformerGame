#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Entity.h"

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

private:
	List<Entity*> entityList;

	SDL_Texture* gEnemyTexture;
	SDL_Texture* aEnemyTexture;
	SDL_Texture* heartsTexture;
	SDL_Texture* coinsTexture;

	Entity* gEnemy;
	Entity* aEnemy;
	Entity* heart;
	Entity* coin;
};

#endif // _ENTITYMANAGER_H_