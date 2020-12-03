#include "Hearts.h"
#include "App.h"
#include "Render.h"
#include "Player.h"

Hearts::Hearts(fPoint position, SDL_Texture* texture, Type type) : Entity(position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 0, 8, 8 });

	currentAnimation = &idleAnimation;
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