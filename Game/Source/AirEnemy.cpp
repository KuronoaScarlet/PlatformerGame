#include "AirEnemy.h"
#include "App.h"
#include "Render.h"
#include "Collisions.h"
#include "Collider.h"
#include "FadeToBlack.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "map.h"
#include "Pathfinding.h"
#include "Audio.h"
#include "EntityManager.h"

AirEnemy::AirEnemy(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 16, 12, 12 });
	idleAnimation.PushBack({ 15, 16, 12, 12 });
	idleAnimation.loop = true;
	idleAnimation.speed = 0.2f;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 10, 8 }), Collider::Type::ENEMY, listener);

	hitFx = app->audio->LoadFx("Assets/Audio/FX/hit.wav");
}

bool AirEnemy::Start()
{
	return true;
}

bool AirEnemy::Update(float dt)
{
	if (app->entityManager->playerData.godMode == false || app->entityManager->playerData.pauseCondition == false)
	{
		if (Sonar(app->entityManager->playerData.position))
		{
			//If player move
			fPoint positionEnemy = app->map->WorldToMap(position.x, position.y);
			fPoint positionPlayer = app->map->WorldToMap(app->entityManager->playerData.position.x, app->entityManager->playerData.position.y);


			//Cerate Path
			CreatePathEnemy(positionEnemy, positionPlayer);
			int i = GetCurrentPositionInPath(positionEnemy);

			//Move Enemy
			if (lastPathEnemy->At(i + 1) != NULL)
			{

				if (app->entityManager->playerData.position.x < position.x)
				{
					position.x -= 1;
					if (app->entityManager->playerData.position.y >= position.y)	position.y += 0.5f;
					if (app->entityManager->playerData.position.y < position.y)	position.y -= 0.5f;

				}
				else if (app->entityManager->playerData.position.x > position.x)
				{
					position.x += 1;
					if (app->entityManager->playerData.position.y >= position.y)	position.y += 0.5f;
					if (app->entityManager->playerData.position.y < position.y)	position.y -= 0.5f;
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
	}
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
	if (coll->type == Collider::Type::PLAYER && app->entityManager->playerData.hit == false)
	{
		app->entityManager->playerData.hit = true;
		app->audio->PlayFx(hitFx);
		app->entityManager->playerData.lives--;

		if (app->entityManager->playerData.lives == 0)
		{
			if (app->scene1->active == true)
			{
				app->fade->Fade((Module*)app->scene1, (Module*)app->deathScreen, 1);
			}
			if (app->scene2->active == true)
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->deathScreen, 1);
			}
			if (app->scene3->active == true)
			{
				app->fade->Fade((Module*)app->scene3, (Module*)app->deathScreen, 60);
			}
			if (app->scene4->active == true)
			{
				app->fade->Fade((Module*)app->scene4, (Module*)app->deathScreen, 60);
			}
		}
		if (app->entityManager->playerData.lives != 0)
		{
			if (position.x > app->entityManager->playerData.position.x)
			{
				position.x = collider->rect.x + collider->rect.w + 6;
			}
			if (position.x < app->entityManager->playerData.position.x)
			{
				position.x = collider->rect.x - collider->rect.w - 9;
			}
		}
	}
	if (coll->type == Collider::Type::PLAYERFOOT)
	{
		pendingToDelete = true;
		collider->pendingToDelete = true;

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

