#include "Hearts.h"
#include "App.h"
#include "Render.h"
#include "Collisions.h"
#include "Audio.h"
#include "EntityManager.h"

Hearts::Hearts(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 0, 8, 8 });

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 8, 8 }), Collider::Type::HEART, listener);
	heartFx = app->audio->LoadFx("Assets/Audio/FX/heart.wav");
	
}

bool Hearts::Start()
{
	
	return true;
}

bool Hearts::Update(float dt)
{
	currentAnimation->Update();
	return true;
}

bool Hearts::Draw()
{
	SDL_Rect rectHearts;
	rectHearts = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rectHearts);

	return true;
}

void Hearts::Collision(Collider* coll)
{
	if (coll->type == Collider::Type::PLAYER && app->entityManager->playerData.hit == false)
	{
		app->entityManager->playerData.hit = true;
		app->entityManager->playerData.lives++;
		pendingToDelete = true;
		collider->pendingToDelete = true;
		app->audio->PlayFx(heartFx);
	}
}

void Hearts::CleanUp()
{

}