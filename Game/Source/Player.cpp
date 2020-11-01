#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Map.h"


#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");
}

// Destructor
Player::~Player()
{}

// Called before the first frame
bool Player::Start()
{
	playerd.texture = app->tex->Load("Assets/textures/Player.png");


	/*
		for (int i = 0; i < 6; i++)
		{
			walkAnim->PushBack({ 0 + (12 * i),0, 12, 11 });
		}*/



	playerd.posx = 50.0f;
	playerd.posy = 689.0f;
	app->render->camera.x = -10;
	app->render->camera.y = playerd.posy - 2105.0f;



	return true;
}

// Called before render is available
bool Player::Awake()
{
	LOG("Loading Player");
	idleAnim.loop = true;
	idleAnim.speed = 0.04f;
	idleAnim.PushBack({ 0, 0, 12, 11 });
	playerd.currentAnim = &idleAnim;
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
	//playerd.currentAnim->Update();

	if (playerd.posy < 689.0f && godmode == false)
	{
		playerd.vely += gravity;
		playerd.posx += playerd.velx;
		playerd.posy += playerd.vely;
	}


	if (playerd.posy >= 689.0f && godmode == false)
	{
		playerjumping = true;
	}


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{
		playerd.posx = 50.0f;
		playerd.posy = 689.0f;
		app->render->camera.x = -10;
		app->render->camera.y = -1416.0f;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
	{
		playerd.posx -= 1;
		if (playerd.posx >= 200.0f) {
			app->render->camera.x += 3;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT)
	{
		playerd.posx = 50.0f;
		playerd.posy = 689.0f;
		app->render->camera.x = -10;
		app->render->camera.y = -1416.0f;
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->map->ShowCollider();

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_REPEAT)
	{
		if (godmode == true)
		{
			godmode = false;
		}
		else
		{
			godmode = true;
			app->render->camera.x = playerd.posx;
			app->render->camera.x = playerd.posy;
		}

	}

	//PlayerMovement
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		playerd.posx -= 1;
		if (playerd.posx >= 200.0f)
		{
			app->render->camera.x += 3;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		playerd.posx += 1;
		if (playerd.posx >= 200.0f) {
			app->render->camera.x -= 3;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godmode == true)
	{
		playerd.posy -= 1;
		app->render->camera.y += 3;

	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godmode == true)
	{
		playerd.posy += 1;
		app->render->camera.y -= 3;

	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (doublejump == true) {
			playerd.vely = -4.0f;
			doublejump = false;
		}
		if (playerjumping == true) {
			playerjumping = false;
			playerd.vely = -6.0f;
			playerd.posy += playerd.vely;
			doublejump = true;
			//Player->vely - 15;
		}


	}
	/*if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;*/

	
	app->render->DrawTexture(playerd.texture, playerd.posx, playerd.posy);



	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;

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
	playerd.posx = play.attribute("x").as_int(0);
	playerd.posy = play.attribute("y").as_int(0);

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(playerd.posx);
	play.attribute("y").set_value(playerd.posy);

	return true;
}
