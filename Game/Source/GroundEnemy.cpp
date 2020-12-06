#include "GroundEnemy.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "FadeToBlack.h"
#include "Scene.h"
#include "Scene2.h"
#include "map.h"
#include "Audio.h"
#include "Pathfinding.h"

GroundEnemy::GroundEnemy(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 1, 1, 10, 12 });

	walkAnimRight.PushBack({ 13,0, 10, 13 });
	walkAnimRight.PushBack({ 25,0, 10, 13 });
	walkAnimRight.PushBack({ 37,1, 10, 12 });
	walkAnimRight.loop = true;
	walkAnimRight.speed = 0.1f;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 10, 10 }), Collider::Type::ENEMY, listener);
	hitFx = app->audio->LoadFx("Assets/Audio/FX/hit.wav");

	lastPathEnemy = new DynArray<fPoint>();
}

bool GroundEnemy::Start()
{
	return true;
}

bool GroundEnemy::Update(float dt)
{

	vely += gravity;
	position.x += velx;
	position.y += vely;

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
			fPoint nextPositionEnemy = *lastPathEnemy->At(i + 1);
			fPoint nextAuxPositionEnemy = app->map->MapToWorld(nextPositionEnemy.x, nextPositionEnemy.y);
			if (nextAuxPositionEnemy.x < position.x)
			{
				position.x -= 1;
			}
			else if (nextAuxPositionEnemy.x > position.x)
			{
				position.x += 1;
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
	
	currentAnimation = &walkAnimRight;
	currentAnimation->Update();
	collider->SetPos(position.x, position.y);

	return true;
}

bool GroundEnemy::Draw()
{
	SDL_Rect rectEnemy;
	rectEnemy = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rectEnemy);

	return true;
}

void GroundEnemy::Collision(Collider* coll)
{
	if (coll->type == Collider::Type::PLAYER)
	{
		app->audio->PlayFx(hitFx);
		app->player->deathCondition = true;
		app->player->playerData.playerLives--;

		if (app->player->playerData.playerLives == 0)
		{
			if (app->scene->active == true)
			{
				app->fade->Fade((Module*)app->scene, (Module*)app->deathScreen, 60);
			}
			if (app->scene2->active == true)
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->deathScreen, 60);
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
	if (coll->type == Collider::Type::LEFT_WALL)
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
	}
}

void GroundEnemy::CleanUp()
{

}

bool GroundEnemy::Sonar(fPoint distance)
{
	if (position.DistanceManhattan(distance) < range) return true;

	return false;
}

int GroundEnemy::CalculateDistance(fPoint origin, fPoint destination)
{
	return abs(origin.x - destination.x) + abs(origin.y - destination.y);;
}

void GroundEnemy::CreatePathEnemy(fPoint mapPositionEnemy, fPoint mapPositionDestination)
{
	if (checkDestination->Check(1))
	{
		app->pathFinding->ResetPath(mapPositionEnemy);
		checkDestination->Start();
		app->pathFinding->ComputePathAStar(mapPositionEnemy, mapPositionDestination);
		lastPathEnemy = app->pathFinding->GetLastPath();
	}
}

int GroundEnemy::GetCurrentPositionInPath(fPoint mapPositionEnemy)
{
	int i = 0;
	for (i = 0; i < lastPathEnemy->Count(); i++)
	{
		if (mapPositionEnemy == fPoint({ lastPathEnemy->At(i)->x, lastPathEnemy->At(i)->y })) break;
	}
	return i;
}

