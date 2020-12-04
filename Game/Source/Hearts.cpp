#include "Hearts.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"

Hearts::Hearts(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 0, 8, 8 });

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 8, 8 }), Collider::Type::HEART, listener);

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
	if (coll == app->player->collider)
	{
		app->player->playerData.playerLives++;
		pendingToDelete = true;
		collider->pendingToDelete = true;
	}
}

void Hearts::CleanUp()
{

}