#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "Scene3.h"
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

Scene3::Scene3() : Module()
{
	name.Create("scene");
}

// Destructor
Scene3::~Scene3()
{}

// Called before render is available
bool Scene3::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene3::Start()
{
	app->player->scene3 = true;

	app->player->Init();
	app->player->Start();
	app->render->camera.x = 0;
	app->render->camera.y = -app->player->playerData.position.y + 50;

	if (app->map->Load("scene3.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	/*app->entityManager->AddEntity({ 280.0f, 244.0f }, Entity::Type::GROUND_ENEMY);
	app->entityManager->AddEntity({ 266.0f, 116.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 282.0f, 116.0f }, Entity::Type::COINS);*/

	app->collisions->active = true;
	app->map->active = true;

	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	app->collisions->AddCollider({ 240, 384, 240, 16 }, Collider::Type::DEATH, this);
	app->collisions->AddCollider({ 544, 384, 688, 16 }, Collider::Type::DEATH, this);

	

	//Fonts
	char lookupTable[] = { "! @,_./0123456789$:< ?abcdefghijklmnopqrstuvwxyzA" };
	scoreFont = app->fonts->Load("Assets/Font/rtype_font3.png", lookupTable, 2);
	app->activeFonts++; app->totalFonts++;
	
	firstEntry = false;

	app->title->volumMusic = 0;
	app->audio->Volume(app->title->volumMusic, '1');

	return true;
}

// Called each loop iteration
bool Scene3::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene3::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (app->player->playerData.position.x >= 176.0f && app->player->playerData.position.x <= 1089.0f)
		{
			app->render->camera.x += 60*dt;
			
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->player->playerData.position.x >= 176.0f && app->player->playerData.position.x <= 1089.0f)
		{
			app->render->camera.x -= 180*dt;
			
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
bool Scene3::PostUpdate()
{
	bool ret = true;

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	app->fade->Fade(this, (Module*)app->options, 10);
	ret = false;*/

	return ret;
}

// Called before quitting
bool Scene3::CleanUp()
{
	if (!active)return true;

	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();
	app->entityManager->CleanUp();

	app->player->scene3 = false;

	app->scene1->active = false;

	LOG("Freeing scene");
	return true;
}