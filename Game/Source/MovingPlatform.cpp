#include "MovingPlatform.h"
#include "App.h"
#include "Render.h"
#include "Collisions.h"
#include "Collider.h"
#include "FadeToBlack.h"
#include "Scene1.h"
#include "Scene2.h"
#include "map.h"
#include "Audio.h"
#include "Pathfinding.h"
#include "EntityManager.h"

MovingPlatform::MovingPlatform(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.PushBack({ 0, 0, 47, 12 });

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 47, 1 }), Collider::Type::FLOOR, listener);

}

bool MovingPlatform::Start()
{
	return true;
}

bool MovingPlatform::Update(float dt)
{
	if (app->entityManager->playerData.pauseCondition == false)
	{
		if (position.x == 768)
		{
			ping = true;
			pong = false;
		}
		if (position.x == 1104)
		{
			pong = true;
			ping = false;
		}
		if (ping == true)
		{
			position.x += 1;
		}
		if (pong == true)
		{
			position.x -= 1;
		}
		collider->SetPos(position.x, position.y);
	}


	return true;
}

bool MovingPlatform::Draw()
{
	SDL_Rect rectEnemy;
	rectEnemy = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rectEnemy);

	return true;
}

void MovingPlatform::Collision(Collider* coll)
{

}

void MovingPlatform::CleanUp()
{

}