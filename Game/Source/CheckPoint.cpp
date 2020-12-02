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

CheckPoint::CheckPoint() : Module()
{
	name.Create("checkpoint");
	idleAnim.loop = true;
	idleAnim.PushBack({ 0, 0, 12, 32 });
	idleAnim.PushBack({ 14,0, 12, 32 });
	idleAnim.PushBack({ 28,0, 13, 32 });
	idleAnim.PushBack({ 43,0, 12, 32 });
	idleAnim.PushBack({ 57,0, 12, 32 });
	idleAnim.PushBack({ 70,0, 12, 32 });
	idleAnim.PushBack({ 85,0, 12, 32 });
	idleAnim.PushBack({ 101,0, 12, 32 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	activated.PushBack({ 0, 32, 12, 32 });
	activated.PushBack({ 14,32, 12, 32 });
	activated.PushBack({ 28,32, 13, 32 });
	activated.PushBack({ 43,32, 12, 32 });
	activated.PushBack({ 57,32, 12, 32 });
	activated.PushBack({ 70,32, 12, 32 });
	activated.PushBack({ 85,32, 12, 32 });
	activated.PushBack({ 101,32, 12, 32 });
	activated.loop = true;
	activated.speed = 0.1f;


}

CheckPoint::~CheckPoint()
{

}

bool CheckPoint::Start()
{
	InitialPos();
	texture = app->tex->Load("Assets/Textures/check_point.png");
	currentAnim = &idleAnim;
	SDL_Rect colCheckPoint = { position.x, position.y, 12, 32 };
	collider = app->collisions->AddCollider(colCheckPoint, Collider::Type::CHECKPOINT, this);
	return true;
	
}
bool CheckPoint::Awake()
{
	LOG("Loading CheckPoint");

	bool ret = true;

	return ret;
}



// Called each loop iteration
bool CheckPoint::PreUpdate()
{
	return true;
}

bool CheckPoint::Update(float dt)
{
	
	if(on == false)
	{
		currentAnim = &idleAnim;
	}
	else
	{
	
		currentAnim = &activated;
	}
	
	currentAnim->Update();
	return true;
}

bool CheckPoint::PostUpdate()
{
	SDL_Rect rectCheckPoint;
	rectCheckPoint = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x,position.y, &rectCheckPoint);
	return true;
}

bool CheckPoint::CleanUp()
{
	return true;
}
void CheckPoint::InitialPos()
{
	if (app->player->scene1 == true)
	{
		
	}
	if (app->player->scene2 == true)
	{
		position.x = 464.0f;//50
		position.y = 257.0f;//670.0
	}

}
