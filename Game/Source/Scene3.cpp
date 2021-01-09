#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "Scene3.h"
#include "Map.h"
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
	app->timer = 120;
	app->entityManager->AddEntity({ 60.0f, 260.0f }, Entity::Type::PLAYER);

	app->render->camera.y = -app->entityManager->playerData.position.y + 50;
	app->render->camera.x = 0;

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
	app->entityManager->AddEntity({ 768.0f, 240.0f }, Entity::Type::MOVINGPLATFORM);
	app->entityManager->AddEntity({ 736.0f, 160.0f }, Entity::Type::CHECKPOINT);
	app->entityManager->AddEntity({ 96.0f, 240.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 128.0f, 208.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 224.0f, 176.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 384.0f, 64.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 528.0f, 128.0f }, Entity::Type::GROUND_ENEMY);
	app->entityManager->AddEntity({ 464.0f, 272.0f }, Entity::Type::HEARTS);
	app->entityManager->AddEntity({ 496.0f, 288.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 512.0f, 288.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 432.0f, 256.0f }, Entity::Type::AIR_ENEMY);
	app->entityManager->AddEntity({ 800.0f, 224.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1008.0f, 128.0f }, Entity::Type::AIR_ENEMY);
	app->entityManager->AddEntity({ 912.0f, 176.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 960.0f, 144.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 992.0f, 144.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1008.0f, 160.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1024.0f, 176.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1024.0f, 192.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1024.0f, 208.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1136.0f, 176.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1168.0f, 128.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1136.0f, 64.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1232.0f, 48.0f }, Entity::Type::HEARTS);
	app->entityManager->AddEntity({ 1200.0f, 32.0f }, Entity::Type::AIR_ENEMY);

	app->collisions->active = true;
	app->map->active = true;

	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	app->collisions->AddCollider({ 240, 384, 240, 16 }, Collider::Type::DEATH, this);
	app->collisions->AddCollider({ 544, 384, 688, 16 }, Collider::Type::DEATH, this);

	app->collisions->AddCollider({ 1248, 32, 16, 32 }, Collider::Type::WIN, this);

	

	//Fonts
	char lookupTable[] = { "! @,_./0123456789$:< ?abcdefghijklmnopqrstuvwxyzA" };
	scoreFont = app->fonts->Load("Assets/Font/rtype_font3.png", lookupTable, 2);
	app->activeFonts++; app->totalFonts++;

	if (app->loadingGame == true)
	{
		app->LoadGameRequest();
		app->loadingGame = false;
	}
	
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
		if (app->entityManager->playerData.position.x >= 176.0f && app->entityManager->playerData.position.x <= 1100.0f)
		{
			app->render->camera.x += 160*dt;
			
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->entityManager->playerData.position.x >= 176.0f && app->entityManager->playerData.position.x <= 1100.0f)
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
bool Scene3::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene3::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();

	app->scene3->active = false;

	LOG("Freeing scene");
	return true;
}
