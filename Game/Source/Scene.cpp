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
#include "EntityManager.h"
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
	app->player->scene1 = true;

	app->player->Init();
	app->player->Start();

	app->entitymanager->AddEntity({ 260.0f, 244.0f }, Entity::Type::GROUND_ENEMY);
	app->entitymanager->AddEntity({ 166.0f, 210.0f }, Entity::Type::HEARTS);
	app->entitymanager->AddEntity({ 266.0f, 116.0f }, Entity::Type::COINS);
	app->entitymanager->AddEntity({ 282.0f, 116.0f }, Entity::Type::COINS);

	app->collisions->active = true;
	app->map->active = true;
	
	app->map->Load("scene1.tmx");

	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

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
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (app->player->playerData.position.x >= 176.0f && app->player->playerData.position.x <= 192.0f)
		{
			app->render->camera.x += 2;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->player->playerData.position.x >= 176.0f && app->player->playerData.position.x <= 192.0f)
		{
			app->render->camera.x -= 2;
		}
	}
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