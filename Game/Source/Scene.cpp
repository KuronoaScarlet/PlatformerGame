#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	player->texture = app->tex->Load("Assets/textures/player.png");
	app->map->Load("Map.tmx");
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	
	player->posx = 50.0f;
	player->posy = 689.0f;
	app->render->camera.x = -10;
	app->render->camera.y = player->posy - 2105.0f;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (player->posy < 689.0f && godmode == false)
	{
		player->vely += gravity;
		player->posx += player->velx;
		player->posy += player->vely;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{
		player->posx = 50.0f;
		player->posy = 689.0f;
		app->render->camera.x = -10;
		app->render->camera.y = -1416.0f;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
	{
		player->posx -= 1;
		if (player->posx >= 200.0f) {
			app->render->camera.x += 3;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT)
	{
		player->posx = 50.0f;
		player->posy = 689.0f;
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
		} else
		{
			godmode = true;
			app->render->camera.x = player->posx;
			app->render->camera.x = player->posy;
		}

	}

	//PlayerMovement
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{		
		player->posx -= 1;
		if (player->posx >= 200.0f) 
			{
			app->render->camera.x += 3;
			}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player->posx += 1;
		if (player->posx >= 200.0f) {
			app->render->camera.x -= 3;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godmode == true)
	{
		player->posy -= 1;
		app->render->camera.y += 3;

	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godmode == true)
	{
		player->posy += 1;
		app->render->camera.y -= 3;

	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		player->vely = -6.0f;
		player->posy += player->vely;
		//player->vely - 15;
		

	}
	/*if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;*/

	app->map->Draw();
	app->render->DrawTexture(player->texture, player->posx, player->posy);//380 100

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::LoadState(pugi::xml_node& data)
{
	pugi::xml_node play = data.child("position");
	player->posx = play.attribute("x").as_int(0);
	player->posy = play.attribute("y").as_int(0);

	return true;
}

bool Scene::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(player->posx);
	play.attribute("y").set_value(player->posy);

	return true;
}
