#include "Coins.h"
#include "App.h"
#include "Render.h"
#include "Player.h"

Coins::Coins(fPoint position, SDL_Texture* texture, Type type) : Entity(position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 1, 1, 10, 10 });
	idleAnimation.PushBack({ 12,1, 10, 10 });
	idleAnimation.PushBack({ 21,1, 10, 10 });
	idleAnimation.PushBack({ 28,1, 10, 10 });
	idleAnimation.PushBack({ 35,1, 10, 10 });
	idleAnimation.PushBack({ 1,13, 10, 10 });
	idleAnimation.PushBack({ 12,13, 10, 10 });
	idleAnimation.PushBack({ 22,13, 10, 10 });

	idleAnimation.loop = true;
	idleAnimation.speed = 0.15f;

	currentAnimation = &idleAnimation;
}

bool Coins::Start()
{
	return true;
}

bool Coins::Update(float dt)
{
	currentAnimation->Update();
	return true;
}

bool Coins::Draw()
{
	SDL_Rect rectCoins;
	rectCoins = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rectCoins);

	return true;
}