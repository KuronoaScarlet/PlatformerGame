#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Logo.h"
#include "Intro.h"
#include "Title.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Map.h"
#include "Player.h"
#include "Options.h"
#include "EntityManager.h"
#include "CheckPoint.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "DeathScreen.h"
#include "Fonts.h"
#include "WinScreen.h"
#include "Pathfinding.h"


#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	
	frames = 0;

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	logo = new Logo();
	title = new Title();
	intro = new Intro();
	scene1 = new Scene1();
	scene2 = new Scene2();
	scene3 = new Scene3();
	scene4 = new Scene4();
	map = new Map();
	player = new Player();
	options = new Options();
	entityManager = new EntityManager();
	fade = new FadeToBlack();
	collisions = new Collisions(false);
	deathScreen = new DeathScreen();
	winScreen = new WinScreen();
	fonts = new Fonts();
	pathFinding = new PathFinding();
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp

	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(logo);
	AddModule(intro);
	AddModule(title);
	AddModule(scene1);
	AddModule(scene2);
	AddModule(scene3);
	AddModule(scene4);
	AddModule(map);
	AddModule(player);
	AddModule(options);
	AddModule(entityManager);
	AddModule(fade);
	AddModule(deathScreen);
	AddModule(winScreen);
	AddModule(fonts);
	AddModule(pathFinding);

	// Render last to swap buffer

	AddModule(collisions);
	AddModule(render);

	intro->active = false;
	title->active = false;
	scene1->active = false;
	scene2->active = false;
	scene3->active = false;
	scene4->active = false;
	player->active = false;
	options->active = false;
	deathScreen->active = false;
	winScreen->active = false;
	
}

App::~App()
{
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();

	configFile.reset();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

bool App::Awake()
{
	

	bool ret = LoadConfig();

	if (ret == true)
	{
		titl.Create(configApp.child("title").child_value());
		win->SetTitle(titl.GetString());

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	pugi::xml_parse_result result = saveLoadFile.load_file("save_game.xml");
	if (result != NULL)
	{
		saveLoadNode = saveLoadFile.child("save");
		fileSaved = true;
	}
	if (result == NULL)
	{
		fileSaved = false;
	}

	
	return ret;
}

bool App::Start()
{
	PERF_START(perfTimer);
	bool ret = true;
	frameTime.Start();
	lastSecond.Start();
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if(item->data->active == true)
			ret = item->data->Start();

		item = item->next;
	}
	PERF_PEEK(perfTimer);
	caped = false;
	timer = 100;
	return ret;
}

bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		caped = !caped;
	}

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		debugButton = !debugButton;
	}
	if(scene1->active || scene2->active|| scene3->active || scene4->active)	
		if(!entityManager->playerData.pauseCondition)	timer -= 0.016f;
	FinishUpdate();

	return ret;
}

bool App::LoadConfig()
{
	bool ret = true;

	pugi::xml_parse_result result = configFile.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = configFile.child("config");
		configApp = config.child("app");
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	fpsCount++;
	lastSecFrameCnt++;

	// L08: DONE 4: Calculate the dt: differential time since last frame
	dt = frameTime.ReadSec();
	frameTime.Start();
	
	startFramesTimeMs = SDL_GetTicks();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	uint32 lastFrameInMs = 0;
	uint32 framesOnLastUpdate = 0;
	//tempFps = SDL_GetTicks() - fps;

	//float average = fpsCount / startTime.ReadSec();

	//frameSec += dt;
	if (lastSecond.ReadSec() > 1.0f)
	{
		prevLastSecFrameCnt = lastSecFrameCnt;
		lastSecFrameCnt = 0;
		lastSecond.Start();
	}
	fpsAverageSinceStart = fpsCount / startTime.ReadSec();

	if (dt < 1000 / 30 && cappedFrameRate)
	{
		float delay = 1000 / 30 - dt;
		SDL_Delay(delay);
	}

	static char title[256];
	sprintf_s(title, 256, "FPS: %d | AVG FPS %.2f | Last Frame in ms: %.1f(dt) | VSync = On ",
		prevLastSecFrameCnt, fpsAverageSinceStart, dt * 1000.0);
	app->win->SetTitle(title);
	
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return titl.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	saveGameRequested = true;
}

bool App::LoadGame()
{
	bool ret = true;

	pugi::xml_document savedGame;

	pugi::xml_parse_result result = savedGame.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load saved game xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		//General Node
		pugi::xml_node generalNode = savedGame.child("save");

		//Entities
		pugi::xml_node entities = generalNode.child("entities");

		//Render
		pugi::xml_node render = generalNode.child("render");

		//LoadRequests
		app->entityManager->LoadState(entities);
		app->render->LoadState(render);
	}

	loadGameRequested = false;

	return ret;
}

bool App::SaveGame() const
{
	bool ret = true;
	
	saveGameRequested = false;

	ListItem<Module*>* item;
	item = modules.start;

	pugi::xml_document newSave;
	pugi::xml_node save;

	save = newSave.append_child("save");

	pugi::xml_node entities = save.append_child("entities");
	app->entityManager->SaveState(entities);

	pugi::xml_node render = save.append_child("render");
	app->render->SaveState(render);
	
	newSave.save_file("save_game.xml");
	

	return ret;
}