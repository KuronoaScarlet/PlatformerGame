#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Enemies.h"
#include "Map.h"
#include "Collisions.h"


#include "Defs.h"
#include "Log.h"

Enemies::Enemies() : Module()
{
	name.Create("enemy");
	idleAnim.loop = true;
	idleAnim.PushBack({ 1, 1, 10, 12 });


	walkAnimRight.PushBack({ 13,0, 10, 13 });
	walkAnimRight.PushBack({ 25,0, 10, 13 });
	walkAnimRight.PushBack({ 37,1, 10, 12 });
	walkAnimRight.loop = true;
	walkAnimRight.speed = 0.1f;
	
	//deadAnimation.PushBack({ 2, 18, 12, 10 });
	jumpAnim.loop = true;


}

// Destructor
Enemies::~Enemies()
{}

// Called before the first frame
bool Enemies::Start()
{
	InitialPos();
	enemyd.texture = app->tex->Load("Assets/Textures/Enemy.png");
	enemyd.currentAnim = &idleAnim;

	SDL_Rect colE = { enemyd.position.x, enemyd.position.y, 10, 12 };
	SDL_Rect colenemyUp = { enemyd.position.x+2, enemyd.position.y+15, 6, 2 };
	collider = app->collisions->AddCollider(colE, Collider::Type::ENEMY, this);
	killEnemy = app->collisions->AddCollider(colenemyUp, Collider::Type::ENEMY_UP, this);

	

	return true;
}

// Called before render is available
bool Enemies::Awake()
{
	LOG("Loading Player");

	bool ret = true;

	return ret;
}



// Called each loop iteration
bool Enemies::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Enemies::Update(float dt)
{
	//enemyd.position.x += 1;
	enemyd.currentAnim = &walkAnimRight;
	enemyd.currentAnim->Update();
	collider->SetPos(enemyd.position.x, enemyd.position.y);
//	killEnemy->SetPos(enemyd.position.x, enemyd.position.y + 2);
	
	return true;
}

// Called each loop iteration
bool Enemies::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectPlayer;
	rectPlayer = enemyd.currentAnim->GetCurrentFrame();
	app->render->DrawTexture(enemyd.texture, enemyd.position.x, enemyd.position.y, &rectPlayer);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Enemies::CleanUp()
{
	LOG("Freeing Player");

	return true;
}

bool Enemies::LoadState(pugi::xml_node& data)
{
	pugi::xml_node play = data.child("position");
	enemyd.position.x = play.attribute("x").as_int(0);
	enemyd.position.y = play.attribute("y").as_int(0);

	return true;
}

bool Enemies::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(enemyd.position.x);
	play.attribute("y").set_value(enemyd.position.y);

	return true;
}

bool Enemies::CheckCollision(int x, int y)
{
	iPoint posMapPlayer;

	/*if (godMode == false)
	{
		if (app->map->data.layers.At(2)->data->Get(posMapPlayer.x, posMapPlayer.y) != 0)
		{
			return true;
		}
	}*/

	return false;
}

void Enemies::OnCollision(Collider* a, Collider* b)
{
	if (a == collider)
	{
		if (b->type == Collider::Type::FLOOR)
		{
			onGround = true;
			enemyd.position.y = enemyd.position.y;
		}
		if (b->type == Collider::Type::LEFT_WALL)
		{
			enemyd.position.x -= 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::RIGHT_WALL)
		{
			enemyd.position.x += 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::ROOF)
		{
			enemyd.position.y += 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::WIN)
		{
			InitialPos();

		}
	}
}
void Enemies::InitialPos()
{
	enemyd.position.x = 240.0f;//50
	enemyd.position.y = 244.0f;//670.0
}