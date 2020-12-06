#ifndef _GROUNDENEMY_H_
#define _GROUNDENEMY_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "Timer.h"
#include "DynArray.h"

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

	// PathFinding
	bool Radar(fPoint origin);
	void CreatePathEnemy(fPoint origin, fPoint destination);
	int GetCurrentPositionInPath(fPoint mapPositionEnemy);
	void MoveEnemy(fPoint nextAuxPositionEenemy, fPoint mapPositionEnemy);
	int CalculateDistance(fPoint origin, fPoint destination);

	int range = 100;
	bool isDetected = false;
	bool returning;

	Timer* checkDestination = new Timer();
	fPoint destination;
	fPoint positionInitial;

	DynArray<fPoint>* lastPathEnemy;

};

#endif // _GROUNDENEMY_H_