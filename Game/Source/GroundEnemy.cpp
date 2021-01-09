#include "GroundEnemy.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "FadeToBlack.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "map.h"
#include "Audio.h"
#include "Pathfinding.h"
#include "EntityManager.h"

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
	if (app->entityManager->playerData.godMode == false || app->entityManager->playerData.pauseCondition == false)
	{
		vely += gravity;
		position.x += velx;
		position.y += vely;

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
				fPoint nextPositionEnemy = *lastPathEnemy->At(i + 1);
				fPoint nextAuxPositionEnemy = app->map->MapToWorld(nextPositionEnemy.x, nextPositionEnemy.y);
				if (nextAuxPositionEnemy.x < position.x)
				{
					position.x -= 60*dt;
				}
				else if (nextAuxPositionEnemy.x > position.x)
				{
					position.x += 60*dt;
				}
			}
		}
		else
		{

			if (timer <= 75)
			{
				position.x += 30.0f * dt;
				timer++;
			}
			if (timer >= 75 && timer <= 150)
			{
				position.x -= 30.0f * dt;
				timer++;
			}
			if (timer == 150)
				timer = 0;

		}


		currentAnimation = &walkAnimRight;
		currentAnimation->Update();
		collider->SetPos(position.x, position.y);
	}


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
	if (coll->type == Collider::Type::PLAYER && app->entityManager->playerData.hit == false)
	{
		app->entityManager->playerData.hit = true;
		app->audio->PlayFx(hitFx);
		app->entityManager->playerData.lives--;

		if (app->entityManager->playerData.lives == 0)
		{
			if (app->scene1->active == true)
			{
				app->fade->Fade((Module*)app->scene1, (Module*)app->deathScreen, 60);
			}
			if (app->scene2->active == true)
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->deathScreen, 60);
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

