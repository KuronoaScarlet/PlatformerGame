#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "Scene1.h"
#include "Map.h"
#include "Player.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Pathfinding.h"
#include "Title.h"

#include "Defs.h"
#include "Log.h"

Scene1::Scene1() : Module()
{
	name.Create("scene");
}

// Destructor
Scene1::~Scene1()
{}

// Called before render is available
bool Scene1::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene1::Start()
{
	app->timer = 100;
	app->entityManager->AddEntity({ 60.0f, 260.0f }, Entity::Type::PLAYER);

	app->render->camera.y = -app->entityManager->playerData.position.y + 30;
	app->render->camera.x = 0;

	if (app->map->Load("scene1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	app->entityManager->AddEntity({ 280.0f, 244.0f }, Entity::Type::GROUND_ENEMY);
	app->entityManager->AddEntity({ 266.0f, 116.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 282.0f, 116.0f }, Entity::Type::COINS);

	app->collisions->active = true;
	app->map->active = true;

	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	app->collisions->AddCollider({ 416, 64, 15, 15 }, Collider::Type::WIN, this);

	//Fonts
	char lookupTable[] = { "! @,_./0123456789$:< ?abcdefghijklmnopqrstuvwxyzA" };
	scoreFont = app->fonts->Load("Assets/Font/rtype_font3.png", lookupTable, 2);
	app->activeFonts++; app->totalFonts++;
	
	firstEntry = false;

	return true;
}

// Called each loop iteration
bool Scene1::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene1::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (app->entityManager->playerData.position.x >= 176.0f && app->entityManager->playerData.position.x <= 192.0f)
		{
			app->render->camera.x += 60 * dt;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->entityManager->playerData.position.x >= 176.0f && app->entityManager->playerData.position.x <= 192.0f)
		{
			app->render->camera.x -= 170*dt;
		}
	}
	app->map->Draw();
	app->map->LoadColliders();

	//Score
	app->fonts->BlitText(3, 20, scoreFont, "score:");
	sprintf_s(scoreText, 10, "%4d", app->intro->score);
	app->fonts->BlitText(30, 20, scoreFont, scoreText);

	return true;
}

// Called each loop iteration
bool Scene1::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene1::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->player->CleanUp();
	app->map->CleanUp();

	app->scene1->active = false;

	LOG("Freeing scene");
	return true;
}
