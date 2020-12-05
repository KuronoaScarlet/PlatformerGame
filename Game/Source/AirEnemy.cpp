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

void AirEnemy::CleanUp()
{

}