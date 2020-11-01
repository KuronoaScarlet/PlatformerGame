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

// Called before render is available
bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Player::Start()
{
	Playerd.texture = app->tex->Load("Assets/textures/Player.png");

	idleAnim.loop = true;
	idleAnim.speed = 0.04f;


	/*
		for (int i = 0; i < 6; i++)
		{
			walkAnim->PushBack({ 0 + (12 * i),0, 12, 11 });
		}*/
	idleAnim.PushBack({ 0, 0, 12, 11 });


	Playerd.posx = 50.0f;
	Playerd.posy = 689.0f;
	app->render->camera.x = -10;
	app->render->camera.y = Playerd.posy - 2105.0f;

	Playerd.currentanim = &idleAnim;

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
	

	if (Playerd.posy < 689.0f && godmode == false)
	{
		Playerd.vely += gravity;
		Playerd.posx += Playerd.velx;
		Playerd.posy += Playerd.vely;
	}


	if (Playerd.posy >= 689.0f && godmode == false)
	{
		playerjumping = true;
	}


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{
		Playerd.posx = 50.0f;
		Playerd.posy = 689.0f;
		app->render->camera.x = -10;
		app->render->camera.y = -1416.0f;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
	{
		Playerd.posx -= 1;
		if (Playerd.posx >= 200.0f) {
			app->render->camera.x += 3;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT)
	{
		Playerd.posx = 50.0f;
		Playerd.posy = 689.0f;
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
			app->render->camera.x = Playerd.posx;
			app->render->camera.x = Playerd.posy;
		}

	}

	//PlayerMovement
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		Playerd.posx -= 1;
		if (Playerd.posx >= 200.0f)
		{
			app->render->camera.x += 3;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		Playerd.posx += 1;
		if (Playerd.posx >= 200.0f) {
			app->render->camera.x -= 3;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godmode == true)
	{
		Playerd.posy -= 1;
		app->render->camera.y += 3;

	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godmode == true)
	{
		Playerd.posy += 1;
		app->render->camera.y -= 3;

	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (doublejump == true) {
			Playerd.vely = -4.0f;
			doublejump = false;
		}
		if (playerjumping == true) {
			playerjumping = false;
			Playerd.vely = -6.0f;
			Playerd.posy += Playerd.vely;
			doublejump = true;
			//Player->vely - 15;
		}


	}
	/*if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;*/

	app->render->DrawTexture(Playerd.texture, Playerd.posx, Playerd.posy);//380 100
	Playerd.currentanim = &idleAnim;


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
	Playerd.posx = play.attribute("x").as_int(0);
	Playerd.posy = play.attribute("y").as_int(0);

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(Playerd.posx);
	play.attribute("y").set_value(Playerd.posy);

	return true;
}
