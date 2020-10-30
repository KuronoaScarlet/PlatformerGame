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

	player->posx = 100.0f;
	player->posy = 257.0f;

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
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	//PlayerMovement
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		player->posx -= 1;

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		player->posx += 1;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		player->vely = -6.0f;
		player->posy += player->vely;
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
