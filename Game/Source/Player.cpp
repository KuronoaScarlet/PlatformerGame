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
	playerd.texture = app->tex->Load("Assets/Textures/player.png");
	playerd.currentAnim = &idleAnim;

	playerd.livess = app->tex->Load("Assets/Textures/life.png");

	SDL_Rect colP = { playerd.position.x, playerd.position.y, 12, 11 };
	collider = app->collisions->AddCollider(colP, Collider::Type::PLAYER, this);
	
	winCondition = false;
	deathCondition = false;

	InitialPos();



	return true;
}

// Called before render is available
bool Player::Awake()
{
	LOG("Loading Player");
	
	bool ret = true;

	return ret;
}



// Called each loop iteration
bool Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	
	int cameraPositionPlayerY = 360 + (playerd.position.y * -3) + 200;
	
	//app->render->camera.y = -(playerd.position.y)+50; //-2*(playerd.position.y)-50

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		playerd.currentAnim = &idleAnim;
	}
		

	if(onGround == false)
	{
		playerd.vely += gravity;
		playerd.position.x += playerd.velx;
		playerd.position.y += playerd.vely;
	}




	if (onGround == true && godMode == false)
	{
		playerJumping = true;
	}


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{
		InitialPos();
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
	{
		
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT)
	{
		InitialPos();
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		if (debug == false) 
		{
			debug = true;
		}
		else 
		{
			debug = false;
		}
	}
	if (debug == true) {
		app->collisions->DebugDraw();
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
		playerd.position.x -= 1;
		if (godMode == false)
		{
			onGround = false;
		}
		if (playerd.currentAnim != &walkAnimLeft) {
			walkAnimLeft.Reset();
			playerd.currentAnim = &walkAnimLeft;
		}
		
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		playerd.position.x += 1;
		if (godMode == false)
		{
			onGround = false;
		}

		if (playerd.currentAnim != &walkAnimRight) {
			walkAnimRight.Reset();
			playerd.currentAnim = &walkAnimRight;
		}
		
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode == true)
	{
		playerd.position.y -= 1;
		app->render->camera.y += 3;

	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode == true)
	{
		playerd.position.y += 1;
		app->render->camera.y -= 3;

	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && godMode == false)
	{
		if (doubleJump == true && onGround == false) 
		{
			playerd.vely = -4.5f;
			doubleJump = false;
			if (playerd.currentAnim != &jumpAnim) 
			{
				jumpAnim.Reset();
				playerd.currentAnim = &jumpAnim;
			}
		}
		if (playerJumping == true ) 
		{
			
			playerJumping = false;
			playerd.vely = -5.5f;
			playerd.position.y += playerd.vely;			
			doubleJump = true;
			if (playerd.currentAnim != &jumpAnim)
			{
				jumpAnim.Reset();
				playerd.currentAnim = &jumpAnim;
			}
			onGround = false;
			//Player->vely - 15;
		}
		


	}

	playerd.currentAnim->Update();
	if (godMode == false)
	{
		collider->SetPos(playerd.position.x, playerd.position.y + 2);
	}
	
	cameraControl = true;

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectPlayer;
	rectPlayer = playerd.currentAnim->GetCurrentFrame();
	app->render->DrawTexture(playerd.livess, -app->render->camera.x+16, -app->render->camera.y - 120, NULL);//50 281
	app->render->DrawTexture(playerd.texture, playerd.position.x, playerd.position.y, &rectPlayer);
	/*for (int i = 0; i < playerd.playerLives; i++)
	{
		app->render->DrawTexture(playerd.livess, -app->render->camera.x + (i * 30), 0, NULL);
	}*/
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing Player");

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	pugi::xml_node play = data.child("position");
	playerd.position.x = play.attribute("x").as_int(0);
	playerd.position.y = play.attribute("y").as_int(0);

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(playerd.position.x);
	play.attribute("y").set_value(playerd.position.y);

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
			playerd.vely = 0;
			playerd.position.y = playerd.position.y;
		}
		if (b->type == Collider::Type::LEFT_WALL)
		{
			playerd.position.x -= 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::RIGHT_WALL)
		{
			playerd.position.x += 1;
			cameraControl = false;
		}
		if (b->type == Collider::Type::ROOF)
		{
			//playerd.position.y += 1;
			playerd.vely = 0;
			playerd.position.y = b->rect.y + b->rect.h;
			cameraControl = false;
			doubleJump = false;
		}
		if (b->type == Collider::Type::WIN && winCondition == false)
		{
			app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 60);
			winCondition= true;

		}
		if (b->type == Collider::Type::ENEMY && deathCondition == false)
		{
			//InitialPos();
			app->fade->Fade((Module*)app->scene, (Module*)app->scene, 60);
			deathCondition = true;
		}
		if (b->type == Collider::Type::DEATH)
		{
			InitialPos();
			//app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 60);
		}
		if (b->type == Collider::Type::CHECKPOINT)
		{
			app->SaveGameRequest();
		}
		
	}
}
void Player::InitialPos() 
{
	if (scene1 == true) 
	{
		playerd.position.x = 50.0f;//50
		playerd.position.y = 200.0f;//670.0
		playerd.vely = 0;

		app->render->camera.x = 0;//-10
		app->render->camera.y = -playerd.position.y;
	}
	if (scene2 == true)
	{
		app->player->playerd.position.x = 50.0f;//50
		app->player->playerd.position.y = 278.0f;//670.0

		app->render->camera.x = 0;//-10
		app->render->camera.y = (-app->player->playerd.position.y) + 100;
	}
	
}