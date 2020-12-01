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
	name.Create("enemies");
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
	enemyData.texture = app->tex->Load("Assets/Textures/Enemy.png");
	enemyData.currentAnim = &idleAnim;

	SDL_Rect colE = { enemyData.position.x, enemyData.position.y, 10, 8 };
	SDL_Rect colenemyUp = { enemyData.position.x+2, enemyData.position.y+15, 6, 2 };
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
	
	//enemyData.position.x += 0.5f;
	enemyData.currentAnim = &walkAnimRight;
	enemyData.currentAnim->Update();
	collider->SetPos(enemyData.position.x, enemyData.position.y);
	//killEnemy->SetPos(enemyData.position.x, enemyData.position.y + 2);
	
	return true;
}

// Called each loop iteration
bool Enemies::PostUpdate()
{
	bool ret = true;
	if (deathCondition == false)
	{
		
		SDL_Rect rectPlayer;
		rectPlayer = enemyData.currentAnim->GetCurrentFrame();
		app->render->DrawTexture(enemyData.texture, enemyData.position.x, enemyData.position.y, &rectPlayer);
	}
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Enemies::CleanUp()
{
	LOG("Freeing Player");
	active = false;
	return true;
}

bool Enemies::LoadState(pugi::xml_node& data)
{
	pugi::xml_node play = data.child("position");
	enemyData.position.x = play.attribute("x").as_int(0);
	enemyData.position.y = play.attribute("y").as_int(0);

	return true;
}

bool Enemies::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(enemyData.position.x);
	play.attribute("y").set_value(enemyData.position.y);

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
			enemyData.position.y = enemyData.position.y;
		}
		if (b->type == Collider::Type::LEFT_WALL)
		{
			enemyData.position.x -= 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::RIGHT_WALL)
		{
			enemyData.position.x += 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::ROOF)
		{
			enemyData.position.y += 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::WIN)
		{
			InitialPos();

		}
		if (b->type == Collider::Type::PLAYER)
		{
			InitialPos();

		}
	}
	if (a == killEnemy)
	{
		if (b->type == Collider::Type::PLAYERFOOT)
		{
			//enemyData.position.x = -1000.0f;
			//enemyData.position.y = -1000.0f;
		}
	}
}
void Enemies::InitialPos()
{
	enemyData.position.x = 240.0f;//50
	enemyData.position.y = 244.0f;//670.0
}