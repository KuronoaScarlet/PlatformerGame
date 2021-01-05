#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "CheckPoint.h"
#include "Map.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Player.h"


#include "Defs.h"
#include "Log.h"

CheckPoint::CheckPoint(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 0, 12, 32 });
	idleAnimation.PushBack({ 14,0, 12, 32 });
	idleAnimation.PushBack({ 28,0, 13, 32 });
	idleAnimation.PushBack({ 43,0, 12, 32 });
	idleAnimation.PushBack({ 57,0, 12, 32 });
	idleAnimation.PushBack({ 70,0, 12, 32 });
	idleAnimation.PushBack({ 85,0, 12, 32 });
	idleAnimation.PushBack({ 101,0, 12, 32 });
	idleAnimation.loop = true;
	idleAnimation.speed = 0.1f;

	activatedAnimation.PushBack({ 0, 32, 12, 32 });
	activatedAnimation.PushBack({ 14,32, 12, 32 });
	activatedAnimation.PushBack({ 28,32, 13, 32 });
	activatedAnimation.PushBack({ 43,32, 12, 32 });
	activatedAnimation.PushBack({ 57,32, 12, 32 });
	activatedAnimation.PushBack({ 70,32, 12, 32 });
	activatedAnimation.PushBack({ 85,32, 12, 32 });
	activatedAnimation.PushBack({ 101,32, 12, 32 });
	activatedAnimation.loop = true;
	activatedAnimation.speed = 0.1f;


	currentAnimation = &idleAnimation;
	SDL_Rect colCheckPoint = { position.x, position.y, 12, 32 };
	collider = app->collisions->AddCollider(colCheckPoint, Collider::Type::CHECKPOINT, listener);

	checkPointFx = app->audio->LoadFx("Assets/Audio/FX/checkpoint.wav");
}

bool CheckPoint::Start()
{
	return true;
}

bool CheckPoint::Update(float dt)
{
	if (!app->player->pauseCondition)
	{
		if (on == false) currentAnimation = &idleAnimation;
		else currentAnimation = &activatedAnimation;
		currentAnimation->Update();
		return true;
	}
}

bool CheckPoint::Draw()
{
	SDL_Rect rectCheckPoint;
	rectCheckPoint = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x,position.y, &rectCheckPoint);
	return true;
}

void CheckPoint::Collision(Collider* coll)
{
	if (coll == app->player->collider)
	{
		if (on == false) {
			app->SaveGameRequest();
			app->audio->PlayFx(checkPointFx);
		}
		on = true;
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		{
			app->player->playerData.position.x = 50.0f;
			app->player->playerData.position.y = 278.0f;

			app->render->camera.x = 0;
			app->render->camera.y = (-app->player->playerData.position.y) + 100;
		}
	}
}

void CheckPoint::CleanUp()
{
	
}