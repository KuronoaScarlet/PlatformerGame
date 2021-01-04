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
#include "CheckPoint.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "EntityManager.h"

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
	app->player->scene2 = true;

	app->player->Init();
	app->player->Start();

	app->collisions->active = true;
	app->map->active = true;
	
	app->map->Load("scene2.tmx");

	app->audio->PlayMusic("Assets/Audio/music/music_spy.ogg");

	app->collisions->AddCollider({ 1248, 240, 15, 15 }, Collider::Type::WIN, this); 

	//Enemies
	app->entityManager->AddEntity({ 752.0f, 270.0f }, Entity::Type::GROUND_ENEMY);
	app->entityManager->AddEntity({ 752.0f, 112.0f }, Entity::Type::AIR_ENEMY);
	app->entityManager->AddEntity({ 1056.0f, 32.0f }, Entity::Type::AIR_ENEMY);

	//Coins
	app->entityManager->AddEntity({ 128.0f, 256.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 128.0f, 192.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 112.0f, 192.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 192.0f, 48.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 352.0f, 64.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 224.0f, 256.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 240.0f, 240.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 256.0f, 256.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 560.0f, 208.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 608.0f, 208.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 704.0f, 144.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 752.0f, 144.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 448.0f, 96.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 464.0f, 96.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 928.0f, 176.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 944.0f, 176.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 896.0f, 48.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 880.0f, 48.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 880.0f, 32.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1008.0f, 96.0f }, Entity::Type::COINS);
	app->entityManager->AddEntity({ 1024.0f, 96.0f }, Entity::Type::COINS);

	//Hearts
	app->entityManager->AddEntity({ 608.0f, 256.0f }, Entity::Type::HEARTS);

	//Checkpoint
	app->entityManager->AddEntity({ 464.0f, 257.0f }, Entity::Type::CHECKPOINT);
	
	//Fonts
	char lookupTable[] = { "! @,_./0123456789$:< ?abcdefghijklmnopqrstuvwxyzA" };
	scoreFont = app->fonts->Load("Assets/Font/rtype_font3.png", lookupTable, 2);
	app->activeFonts++; app->totalFonts++;

	firstEntry = false;

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
	/*if (app->player->playerData.position.x >= 213.3f && app->player->playerData.position.x <= 1055.0f && app->player->cameraControl == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			app->render->camera.x -= 2;
			if(app->player->cameraControl == false)
				app->render->camera.x += 2000 * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			app->render->camera.x += 1.6;
			if (app->player->cameraControl == false)
				app->render->camera.x -= 2000 * dt;

		}
	}*/

	app->map->Draw();
	app->map->LoadColliders();

	//Score
	app->fonts->BlitText(3, 20, scoreFont, "score:");
	sprintf_s(scoreText, 10, "%4d", app->intro->score);
	app->fonts->BlitText(30, 20, scoreFont, scoreText);

	return true;
}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	

	return ret;
}

// Called before quitting
bool Scene2::CleanUp()
{
	if (!active)return true;

	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();
	app->entityManager->CleanUp();

	app->player->scene2 = false;

	app->scene2->active = false;

	LOG("Freeing scene2");
	return true;
}