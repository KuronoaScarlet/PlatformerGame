#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#define MAX_COLLIDERS 1000

#include "Module.h"
#include "Collider.h"

class Collisions : public Module
{
public:
	Collisions(bool startEnabled);

	// Destructor
	virtual ~Collisions() {}

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	Collider* AddDynamicCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

	// Removes the collider memory and removes it from the colliders array
	void RemoveCollider(Collider* collider);

	// Draws all existing colliders with some transparency
	void DebugDraw();
	void DebugRequest();


	bool matrix[Collider::Type::MAX][Collider::Type::MAX];

private:
	// All existing colliders in the scene
	Collider* colliders[MAX_COLLIDERS] = { nullptr };

	// The collision matrix. Defines the interaction for two collider types
	// If set two false, collider 1 will ignore collider 2


	// Simple debugging flag to draw all colliders
	bool debug = false;
};

#endif // __COLLISIONS_H__
