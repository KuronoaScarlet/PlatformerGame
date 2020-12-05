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

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 10, 8 }), Collider::Type::ENEMY, listener);
	hitFx = app->audio->LoadFx("Assets/Audio/FX/heart.wav");
}

bool GroundEnemy::Start()
{
	return true;
}

bool GroundEnemy::Update(float dt)
{
	app->map->goalAStar = { 13,16 };

	if (app->map->endAStar == false) {

		app->map->PropagateAStar(0);


		/*for (int i = 0; i < app->map->visited.Count(); i++)
		{
			if (app->map->visited.At(i)->data.x == app->map->goalAStar.x && app->map->visited.At(i)->data.y == app->map->goalAStar.y)
			{
				app->map->endAStar = true;
				app->map->ComputePathAStar(app->map->goalAStar.x, app->map->goalAStar.y);
			}
		}*/
	}
	if (app->map->path.At(0) != NULL) {
		int i;
		for (i = 0; i < app->map->path.Count(); i++)
		{
			if (position.x == app->map->path.At(i)->x && position.y == app->map->path.At(i)->y) break;
		}
		iPoint pos = app->map->WorldToMap(position.x, position.y);
		if (pos.x < app->map->path.At(i + 1)->x)
		{
			position.x += 1;
		}
		else
		{
			position.x -= 1;
		}
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
}

void GroundEnemy::CleanUp()
{

}