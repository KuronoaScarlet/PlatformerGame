#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Enemies.h"
#include "Collisions.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->player->Init();
	app->player->Start();

	app->player->scene1 = true;

	//Player start
	app->player->playerd.position.x = 50.0f;//50
	app->player->playerd.position.y = 200.0f;//670.0

	app->render->camera.x = 0;//-10
	app->render->camera.y = -app->player->playerd.position.y;

	//Enemies start
	app->enemy->Init();
	app->enemy->Start();

	app->collisions->active = true;
	app->map->active = true;
	
	app->map->Load("scene1.tmx");

	app->audio->PlayMusic("Assets/Audio/music/music_spy.ogg");

	app->collisions->AddCollider({ 416, 64, 15, 15 }, Collider::Type::WIN, this);
	
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	app->map->Draw();
	app->map->LoadColliders();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene2, 60);

	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	if (!active)return true;

	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();

	app->player->scene1 = false;

	app->scene->active = false;

	LOG("Freeing scene");
	return true;
}