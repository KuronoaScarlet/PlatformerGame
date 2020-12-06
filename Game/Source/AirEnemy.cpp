#include "AirEnemy.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "FadeToBlack.h"
#include "Scene.h"
#include "Scene2.h"
#include "map.h"
#include "Player.h"
#include "Pathfinding.h"

AirEnemy::AirEnemy(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 16, 12, 12 });
	idleAnimation.PushBack({ 15, 16, 12, 12 });
	idleAnimation.loop = true;
	idleAnimation.speed = 0.2f;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 10, 8 }), Collider::Type::ENEMY, listener);
}

bool AirEnemy::Start()
{
	return true;
}

bool AirEnemy::Update(float dt)
{
	if (Sonar(app->player->playerData.position))
	{
		//If player move
		fPoint positionEnemy = app->map->WorldToMap(position.x, position.y);
		fPoint positionPlayer = app->map->WorldToMap(app->player->playerData.position.x, app->player->playerData.position.y);


		//Cerate Path
		CreatePathEnemy(positionEnemy, positionPlayer);
		int i = GetCurrentPositionInPath(positionEnemy);

		//Move Enemy
		if (lastPathEnemy->At(i + 1) != NULL)
		{
			
			if (app->player->playerData.position.x < position.x)
			{
				position.x -= 1;
				if (app->player->playerData.position.y >= position.y)	position.y += 0.5f;
				if (app->player->playerData.position.y < position.y)	position.y -= 0.5f;

			}
			else if (app->player->playerData.position.x > position.x)
			{
				position.x += 1;
				if (app->player->playerData.position.y >= position.y)	position.y += 0.5f;
				if (app->player->playerData.position.y < position.y)	position.y -= 0.5f;
			}
		}
	}
	else
	{

		if (timer <= 100)
		{
			position.x += 0.5f;
			timer++;
		}
		if (timer >= 100 && timer <= 200)
		{
			position.x -= 0.5f;
			timer++;
		}
		if (timer == 200)
			timer = 0;

	}

	currentAnimation = &idleAnimation;
	currentAnimation->Update();
	collider->SetPos(position.x, position.y);

	return true;
}

bool AirEnemy::Draw()
{
	SDL_Rect rectEnemy;
	rectEnemy = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rectEnemy);

	return true;
}

void AirEnemy::Collision(Collider* coll)
{
	if (coll->type == Collider::Type::PLAYER)
	{
		app->player->deathCondition = true;
		app->player->playerData.playerLives--;

		if (app->player->playerData.playerLives == 0)
		{
			if (app->scene->active == true)
			{
				app->fade->Fade((Module*)app->scene, (Module*)app->deathScreen, 1);
			}
			if (app->scene2->active == true)
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->deathScreen, 1);
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
	if (coll->type == Collider::Type::FLOOR)
	{
		position.y = coll->rect.y - coll->rect.h - 9;
		position.y = position.y;
	}
	if (coll->type == Collider::Type::LEFT_WALL)
	{
		position.x = coll->rect.x - coll->rect.w - 10;
		
	}
	if (coll->type == Collider::Type::RIGHT_WALL)
	{
		position.x = coll->rect.x + coll->rect.w + 1;
		
	}
	if (coll->type == Collider::Type::ROOF)
	{
		position.y = coll->rect.y + coll->rect.h;
		
	}
}

void AirEnemy::CleanUp()
{

}
bool AirEnemy::Sonar(fPoint distance)
{
	if (position.DistanceManhattan(distance) < range) return true;

	return false;
}

int AirEnemy::CalculateDistance(fPoint origin, fPoint destination)
{
	return abs(origin.x - destination.x) + abs(origin.y - destination.y);;
}

void AirEnemy::CreatePathEnemy(fPoint mapPositionEnemy, fPoint mapPositionDestination)
{
	if (checkDestination->Check(1))
	{
		app->pathFinding->ResetPath(mapPositionEnemy);
		checkDestination->Start();
		app->pathFinding->ComputePathAStar(mapPositionEnemy, mapPositionDestination);
		lastPathEnemy = app->pathFinding->GetLastPath();
	}
}

int AirEnemy::GetCurrentPositionInPath(fPoint mapPositionEnemy)
{
	int i = 0;
	for (i = 0; i < lastPathEnemy->Count(); i++)
	{
		if (mapPositionEnemy == fPoint({ lastPathEnemy->At(i)->x, lastPathEnemy->At(i)->y })) break;
	}
	return i;
}

