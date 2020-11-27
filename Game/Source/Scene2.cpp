#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "Scene2.h"
#include "Map.h"
#include "Player.h"
#include "Enemies.h"
#include "Collisions.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene2::Scene2() : Module()
{
	name.Create("scene2");
}

// Destructor
Scene2::~Scene2()
{}

// Called before render is available
bool Scene2::Awake()
{
	LOG("Loading Scene2");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene2::Start()
{
	app->player->Init();
	app->player->Start();

	app->player->playerd.position.x = 50.0f;//50
	app->player->playerd.position.y = 200.0f;//670.0

	app->render->camera.x = 0;//-10
	app->render->camera.y = (-app->player->playerd.position.y) - 20;

	/*app->enemy->Init();
	app->enemy->Start();*/

	app->collisions->active = true;
	app->map->active = true;
	
	app->map->Load("scene2.tmx");

	app->audio->PlayMusic("Assets/Audio/music/music_spy.ogg");

	app->collisions->AddCollider({ 416, 64, 15, 15 }, Collider::Type::WIN, this);
	
	

	return true;
}

// Called each loop iteration
bool Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene2::Update(float dt)
{

	app->map->Draw();
	app->map->LoadColliders();

	return true;
}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	

	return ret;
}

// Called before quitting
bool Scene2::CleanUp()
{
	if (!active)return true;

	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();

	app->scene2->active = false;

	LOG("Freeing scene2");
	return true;
}