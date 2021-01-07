#include "Boss.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "FadeToBlack.h"
#include "Scene4.h"
#include "map.h"
#include "Audio.h"
#include "Pathfinding.h"
#include "EntityManager.h"

Boss::Boss(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 162, 197, 138 });
	idleAnimation.PushBack({ 213, 162, 197, 138 });
	idleAnimation.speed = 0.05f;
	
	
	atackAnimation.PushBack({ 13,0, 10, 13 });
	atackAnimation.PushBack({ 25,0, 10, 13 });
	atackAnimation.PushBack({ 37,1, 10, 12 });
	atackAnimation.loop = true;
	atackAnimation.speed = 0.1f;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 197, 138 }), Collider::Type::ENEMY, listener);
	hitFx = app->audio->LoadFx("Assets/Audio/FX/hit.wav");

	lastPathEnemy = new DynArray<fPoint>();
}

bool Boss::Start()
{
	return true;
}

bool Boss::Update(float dt)
{
	
	if (!app->player->pauseCondition)
	{
		if (Sonar(app->player->playerData.position))
		{
			counter += dt;
			if (counter > 50)
			{
				counter = 0;
				//app->entityManager->AddEntity({ 266.0f, 116.0f }, Entity::Type::AIR_ENEMY);
				
			}
			//currentAnimation = &atackAnimation;
		}
	}
	
	currentAnimation->Update();
	collider->SetPos(position.x, position.y);

	

	return true;
}

bool Boss::Draw()
{
	SDL_Rect rectEnemy;
	rectEnemy = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rectEnemy);

	return true;
}

void Boss::Collision(Collider* coll)
{
	if (coll->type == Collider::Type::PLAYER)
	{
		app->audio->PlayFx(hitFx);
		app->player->deathCondition = true;
		app->player->playerData.playerLives--;

		if (app->player->playerData.playerLives == 0)
		{
			if (app->scene4->active == true)
			{
				app->fade->Fade((Module*)app->scene4, (Module*)app->deathScreen, 60);
			}
		}
		if (app->player->playerData.playerLives != 0)
		{
			if (position.x > app->player->playerData.position.x)
			{
				app->player->playerData.position.x = collider->rect.x - collider->rect.w - 9;
			}
			if (position.x < app->player->playerData.position.x)
			{
				app->player->playerData.position.x = collider->rect.x + collider->rect.w + 6;
			}
		}
	}
	if (coll->type == Collider::Type::PLAYERFOOT)
	{
		pendingToDelete = true;
		collider->pendingToDelete = true;

		app->player->playerData.vely = -5.5f;
		app->player->playerData.position.y += app->player->playerData.vely;
	}
	/*if (coll->type == Collider::Type::LEFT_WALL)
	{
		position.x -= 1;
		timer = 100;
	}
	if (coll->type == Collider::Type::RIGHT_WALL)
	{
		position.x += 1;
		timer = 0;
	}
	if (coll->type == Collider::Type::FLOOR)
	{
		position.y = coll->rect.y - coll->rect.h - 9;
		vely = 0;
		position.y = position.y;
	}*/
}

void Boss::CleanUp()
{

}

bool Boss::Sonar(fPoint distance)
{
	if (position.DistanceManhattan(distance) < range) return true;

	return false;
}

int Boss::CalculateDistance(fPoint origin, fPoint destination)
{
	return abs(origin.x - destination.x) + abs(origin.y - destination.y);;
}

void Boss::CreatePathEnemy(fPoint mapPositionEnemy, fPoint mapPositionDestination)
{
	if (checkDestination->Check(1))
	{
		app->pathFinding->ResetPath(mapPositionEnemy);
		checkDestination->Start();
		app->pathFinding->ComputePathAStar(mapPositionEnemy, mapPositionDestination);
		lastPathEnemy = app->pathFinding->GetLastPath();
	}
}

int Boss::GetCurrentPositionInPath(fPoint mapPositionEnemy)
{
	int i = 0;
	for (i = 0; i < lastPathEnemy->Count(); i++)
	{
		if (mapPositionEnemy == fPoint({ lastPathEnemy->At(i)->x, lastPathEnemy->At(i)->y })) break;
	}
	return i;
}

