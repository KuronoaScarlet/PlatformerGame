#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "Scene4.h"
#include "Map.h"
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
	app->stop = false;
	timerr.Start();
	app->entityManager->AddEntity({ 60.0f, 260.0f }, Entity::Type::PLAYER);
	time = 0;

	app->render->camera.y = -app->entityManager->playerData.position.y + 50;
	app->render->camera.x = 0;

	if (app->map->Load("final_boss.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	app->entityManager->AddEntity({ 240.0f, 256.0f }, Entity::Type::GROUND_ENEMY);
	app->entityManager->AddEntity({ 544.0f, 176.0f }, Entity::Type::AIR_ENEMY);
	app->entityManager->AddEntity({ 656.0f, 80.0f }, Entity::Type::AIR_ENEMY);

	app->entityManager->AddEntity({ 416.0f, 144.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 432.0f, 144.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 448.0f, 144.0f }, Entity::Type::COINS);	
	app->entityManager->AddEntity({ 544.0f, 192.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 560.0f, 192.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 576.0f, 192.0f }, Entity::Type::COINS);


	app->entityManager->AddEntity({ 800.0f, 100.0f }, Entity::Type::BOSS);

	app->collisions->active = true;
	app->map->active = true;

	app->audio->PlayMusic("Assets/Audio/Music/scene4_music.ogg");
	coinTexture = app->tex->Load("Assets/Textures/coins.png");

	coinAnimation.loop = true;
	coinAnimation.PushBack({ 1, 1, 10, 10 });
	coinAnimation.PushBack({ 12, 1, 10, 10 });
	coinAnimation.PushBack({ 21, 1, 10, 10 });
	coinAnimation.PushBack({ 28, 1, 10, 10 });
	coinAnimation.PushBack({ 35, 1, 10, 10 });
	coinAnimation.PushBack({ 1, 13, 10, 10 });
	coinAnimation.PushBack({ 12, 13, 10, 10 });
	coinAnimation.PushBack({ 22, 13, 10, 10 });

	coinAnimation.loop = true;
	coinAnimation.speed = 0.15f;

	currentAnimation = &coinAnimation;
	
	if (app->loadingGame == true)
	{
		app->LoadGameRequest();
		app->loadingGame = false;
	}
	
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
	currentAnimation->Update();
	sprintf_s(scoreText, 10, "%4d", app->intro->score);
	app->render->DrawText(app->render->font, scoreText, 6, 48, 50, 5, { 100, 100, 100, 255 });

	return true;
}

// Called each loop iteration
bool Scene4::PostUpdate()
{
	bool ret = true;
	time = 150 - timerr.ReadSec();

	app->render->DrawText(app->render->font, "Timer:", 1000, 10, 50, 5, { 100, 100, 100, 255 });
	sprintf_s(timeText, 10, "%4d", time);
	app->render->DrawText(app->render->font, timeText, 1140, 10, 50, 5, { 100, 100, 100, 255 });

	SDL_Rect rectCoins;
	rectCoins = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(coinTexture, (-app->render->camera.x + 5) / 3, (-app->render->camera.y + 58) / 3, &rectCoins);

	return ret;
}

// Called before quitting
bool Scene4::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();

	app->scene4->active = false;

	LOG("Freeing scene");
	return true;
}
