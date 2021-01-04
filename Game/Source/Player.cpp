#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Map.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "CheckPoint.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Scene2.h"
#include "Title.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");
	idleAnim.loop = true;
	idleAnim.PushBack({ 0, 0, 12, 11 });

    walkAnimRight.PushBack({ 13,0, 12, 11 });
	walkAnimRight.PushBack({ 26,0, 12, 11 });
	walkAnimRight.PushBack({ 39,0, 12, 11 });
	walkAnimRight.PushBack({ 53,0, 14, 11 });
	walkAnimRight.loop = true;
	walkAnimRight.speed = 0.1f;

	walkAnimLeft.PushBack({ 13,12, 12, 11 });
	walkAnimLeft.PushBack({ 26,12, 12, 11 });
	walkAnimLeft.PushBack({ 39,12, 12, 11 });
	walkAnimLeft.PushBack({ 53,12, 14, 11 });
	walkAnimLeft.loop = true;
	walkAnimLeft.speed = 0.1f;

	jumpAnim.PushBack({ 1, 23, 12, 12 });
	jumpAnim.loop = true;
}

// Destructor
Player::~Player()
{}

// Called before the first frame
bool Player::Start()
{
	playerData.texture = app->tex->Load("Assets/Textures/player.png");
	playerData.currentAnim = &idleAnim;

	playerData.livesTexture = app->tex->Load("Assets/Textures/life.png");

	SDL_Rect colP = { playerData.position.x, playerData.position.y, 12, 11 };
	collider = app->collisions->AddCollider(colP, Collider::Type::PLAYER, this);

	SDL_Rect colPFoot = { playerData.position.x+4, playerData.position.y, 11, 8 };
	playerFoot = app->collisions->AddCollider(colPFoot, Collider::Type::PLAYERFOOT, this);
	
	winCondition = false;
	deathCondition = false;

	jumpFx = app->audio->LoadFx("Assets/Audio/FX/jump.wav");
	doubleJumpFx = app->audio->LoadFx("Assets/Audio/FX/double_jump.wav");
	checkPointFx = app->audio->LoadFx("Assets/Audio/FX/checkpoint.wav");
	killingEnemyFx = app->audio->LoadFx("Assets/Audio/FX/enemy_death.wav");

	InitialPos();
	return true;
}

// Called before render is available
bool Player::Awake()
{
	LOG("Loading Player");
	
	return true;
}



// Called each loop iteration
bool Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		playerData.currentAnim = &idleAnim;
	}
	if(onGround == false)
	{
		playerData.vely += gravity;
		playerData.position.x += playerData.velx;
		playerData.position.y += playerData.vely;
	}
	if (onGround == true && godMode == false)
	{
		playerJumping = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{
		if (scene1 == true)
		{
			InitialPos();
		}
		if (scene2 == true)
		{
			app->scene->firstEntry = true;
			app->fade->Fade((Module*)app->scene2, (Module*)app->scene, 60);
			app->entityManager->CleanUp();
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if(scene1==true)
		{
			app->scene2->firstEntry = true;
			app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 60);
			app->entityManager->CleanUp();
		}
		if (scene2 == true)
		{
			InitialPos();
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT)
	{
		InitialPos();
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		if (debug == false) debug = true;
		else debug = false;
	}
	if (debug == true) {
		app->collisions->DebugDraw();
		app->map->DrawPath();
	
	}
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (godMode == true)
		{
			godMode = false;
		}
		else
		{
			godMode = true;
			collider->SetPos(-100, -100);
		}
	}

	//PlayerMovement
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		playerData.position.x -= 7 * dt;
		if (godMode == false)
		{
			onGround = false;
		}
		if (playerData.currentAnim != &walkAnimLeft) {
			walkAnimLeft.Reset();
			playerData.currentAnim = &walkAnimLeft;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		playerData.position.x +=dt;
		if (godMode == false)
		{
			onGround = false;
		}
		if (playerData.currentAnim != &walkAnimRight) {
			walkAnimRight.Reset();
			playerData.currentAnim = &walkAnimRight;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode == true)
	{
		playerData.position.y -= 1;
		app->render->camera.y += 3;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode == true)
	{
		playerData.position.y += 1;
		app->render->camera.y -= 3;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && godMode == false)
	{
		
		if (doubleJump == true && onGround == false) 
		{
			app->audio->PlayFx(doubleJumpFx);
			playerData.vely = -4.5f;
			doubleJump = false;
			if (playerData.currentAnim != &jumpAnim) 
			{
				jumpAnim.Reset();
				playerData.currentAnim = &jumpAnim;
			}
		}
		if (playerJumping == true ) 
		{
			app->audio->PlayFx(jumpFx);
			playerJumping = false;
			playerData.vely = -5.5f;
			playerData.position.y += playerData.vely;			
			doubleJump = true;
			if (playerData.currentAnim != &jumpAnim)
			{
				jumpAnim.Reset();
				playerData.currentAnim = &jumpAnim;
			}
			onGround = false;
		}
	}
	
	
	
	cameraControl = true;

	if (godMode == false)
	{
		collider->SetPos(playerData.position.x, playerData.position.y+2);
		playerFoot->SetPos(playerData.position.x + 1, playerData.position.y + 12);
	}


	playerData.currentAnim->Update();

	if (playerData.position.y <= 230 && playerData.position.y >= 20)
	{
		app->render->camera.y = -playerData.position.y + 30;
	}

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectPlayer;
	rectPlayer = playerData.currentAnim->GetCurrentFrame();
	app->render->DrawTexture(playerData.texture, playerData.position.x, playerData.position.y, &rectPlayer);
	for (int i = 0; i < playerData.playerLives; i++)
	{
		app->render->DrawTexture(playerData.livesTexture, ((-app->render->camera.x + 10) + (i * 32)) / 3, (-app->render->camera.y + 20) / 3, NULL);
	}
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->SaveGameRequest();
		app->title->pauseBool = true;
		if(scene1 == true)
			app->fade->Fade((Module*)app->scene, (Module*)app->pause, 1);
		if (scene2 == true)
			app->fade->Fade((Module*)app->scene2, (Module*)app->pause, 1);
	}
	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		

	

	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing Player");
	active = false;

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	pugi::xml_node play = data.child("position");
	playerData.position.x = play.attribute("x").as_int(0);
	playerData.position.y = play.attribute("y").as_int(0);

	pugi::xml_node level = data.child("level");
	playerData.currentLevel = level.attribute("l").as_int(0);

	if (playerData.currentLevel == 1 && scene2 == true)
	{
		app->fade->Fade((Module*)app->scene2, (Module*)app->scene, 60);
		app->entityManager->CleanUp();
	}
	if (playerData.currentLevel == 2 && scene1 == true)
	{
		app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 60);
		app->entityManager->CleanUp();
	}

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(playerData.position.x);
	play.attribute("y").set_value(playerData.position.y);
	
	pugi::xml_node level = data.child("level");
	if(scene1 == true)
	{
		level.attribute("l").set_value("1");
	}
	if (scene2 == true)
	{
		level.attribute("l").set_value("2");
	}


	return true;
}

bool Player::CheckCollision(int x, int y)
{
	iPoint posMapPlayer;

	if (godMode == false)
	{
		if (app->map->data.layers.At(2)->data->Get(posMapPlayer.x, posMapPlayer.y) != 0)
		{
			return true;
		}
	}

	return false;
}

void Player::OnCollision(Collider* a, Collider* b) 
{
	if (a == collider)
	{
		if (b->type == Collider::Type::FLOOR)
		{
			onGround = true;
			playerData.position.y = b->rect.y - b->rect.h - 9;
			playerData.vely = 0;
			playerData.position.y = playerData.position.y;
		}
		if (b->type == Collider::Type::LEFT_WALL)
		{
			playerData.position.x = b->rect.x - b->rect.w - 10;
			cameraControl = false;
		}
		if (b->type == Collider::Type::RIGHT_WALL)
		{
			playerData.position.x = b->rect.x + b->rect.w + 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::ROOF)
		{
			playerData.vely = 0;
			playerData.position.y = b->rect.y + b->rect.h;
			cameraControl = false;
		}
		if (b->type == Collider::Type::WIN && winCondition == false)
		{
			if (scene1 == true)
			{
				app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 60);
				winCondition = true;
				app->entityManager->CleanUp();
			}
			if (scene2 == true)
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->winScreen, 60);
				winCondition = true;
				app->entityManager->CleanUp();
			}
		}
	}

	if (a == playerFoot)
	{
		if (b->type == Collider::Type::ENEMY)
		{
			playerData.vely = -5.5f;
			playerData.position.y += playerData.vely;
			b->pendingToDelete = true;
			app->audio->PlayFx(killingEnemyFx);
		}
	}
}
void Player::InitialPos() 
{
	if (scene1 == true && app->scene->firstEntry == true) 
	{
		playerData.position.x = 50.0f;
		playerData.position.y = 260.0f;
		playerData.vely = 0;

		app->render->camera.x = 0;
		app->render->camera.y = -playerData.position.y + 50;
	}
	if (scene2 == true && app->scene2->firstEntry == true)
	{
		app->player->playerData.position.x = 50.0f;
		app->player->playerData.position.y = 278.0f;

		app->render->camera.x = 0;
		app->render->camera.y = (-app->player->playerData.position.y) + 100;
	}
}