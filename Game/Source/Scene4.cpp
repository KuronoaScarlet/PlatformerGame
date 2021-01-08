#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "Scene4.h"
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

Scene4::Scene4() : Module()
{
	name.Create("scene");
}

// Destructor
Scene4::~Scene4()
{}

// Called before render is available
bool Scene4::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene4::Start()
{
	app->entityManager->AddEntity({ 60.0f, 260.0f }, Entity::Type::PLAYER);

	app->render->camera.y = -app->entityManager->playerData.position.y + 50;
	app->render->camera.x = 0;

	if (app->map->Load("final_boss.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	app->entityManager->AddEntity({ 800.0f, 100.0f }, Entity::Type::BOSS);

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

	
	return true;
}

// Called each loop iteration
bool Scene4::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene4::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (app->entityManager->playerData.position.x >= 176.0f && app->entityManager->playerData.position.x <= 673.0f)
		{
			app->render->camera.x += 160*dt;
			
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->entityManager->playerData.position.x >= 176.0f && app->entityManager->playerData.position.x <= 673.0f)
		{
			app->render->camera.x -= 200*dt;
			
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
bool Scene4::PostUpdate()
{
	bool ret = true;

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	app->fade->Fade(this, (Module*)app->options, 10);
	ret = false;*/

	return ret;
}

// Called before quitting
bool Scene4::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->player->CleanUp();
	app->map->CleanUp();

	app->scene4->active = false;

	LOG("Freeing scene");
	return true;
}
