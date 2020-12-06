#ifndef _AIRENEMY_H_
#define _AIRENEMY_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "DynArray.h"
#include "Timer.h"

struct SDL_Texture;
struct Collider;

class AirEnemy : Entity
{
public:
	AirEnemy(Module* listener, fPoint position, SDL_Texture* texture, Type type);
	
	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* coll);
	
	void CleanUp();

	uint hitFx = 0;

private:
	Animation idleAnimation;
	Animation walkAnimRight;

	Animation* currentAnimation;

	bool Sonar(fPoint origin);
	void CreatePathEnemy(fPoint origin, fPoint destination);
	int GetCurrentPositionInPath(fPoint mapPositionEnemy);
	int CalculateDistance(fPoint origin, fPoint destination);

	int range = 100;
	bool isDetected = false;
	bool returning;

	Timer* checkDestination = new Timer();
	fPoint destination;
	fPoint positionInitial;

	int timer = 0;

	DynArray<fPoint>* lastPathEnemy;
};

#endif // _GROUNDENEMY_H_