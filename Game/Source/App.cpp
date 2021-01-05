#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Logo.h"
#include "Intro.h"
#include "Title.h"
#include "Scene.h"
#include "Scene2.h"
#include "Map.h"
#include "Player.h"
#include "Options.h"
#include "Pause.h"
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
	PERF_START(perfTimer);
	frames = 0;

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	logo = new Logo();
	title = new Title();
	intro = new Intro();
	scene = new Scene();
	scene2 = new Scene2();
	map = new Map();
	player = new Player();
	options = new Options();
	pause = new Pause();
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
	AddModule(scene);
	AddModule(scene2);
	AddModule(map);
	AddModule(player);
	AddModule(options);
	AddModule(pause);
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
	scene->active = false;
	player->active = false;
	options->active = false;
	pause->active = false;
	scene2->active = false;
	deathScreen->active = false;
	winScreen->active = false;
	PERF_PEEK(perfTimer);
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
	PERF_START(perfTimer);

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
	}
	frameRate60 = configApp.attribute("framerate_cap").as_int(0);
	frameRate30 = 30;
	PERF_PEEK(perfTimer);
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
	frameRate = (caped) ? 1000 / 30 : 1000 / 60;
	fps = SDL_GetTicks();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	uint32 lastFrameInMs = 0;
	uint32 framesOnLastUpdate = 0;
	tempFps = SDL_GetTicks() - fps;

	float average = fpsCount / startTime.ReadSec();

	//frameSec += dt;
	if (frameTime.ReadSec() > 1.0f)
	{
		framesSecond = lastSecFrameCnt;
		lastSecFrameCnt = 0;
		frameTime.Start();
	}

	oldLastFrame = lastFrameInMs;
	lastFrameInMs = lastSecond.Read();
	lastSecond.Start();

	if (frameRate > tempFps)
	{
		SDL_Delay(frameRate - tempFps);
	}
	static char title[256];
	sprintf_s(title, 256, "FPS: %d | AVG FPS %.2f | Last Frame in ms: %d | VSync = On ",
		framesSecond, average, lastFrameInMs);
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

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->LoadState(saveLoadNode.child(item->data->name.GetString()));
		item = item->next;
	}

	loadGameRequested = false;

	return ret;
}

bool App::SaveGame() const
{
	bool ret = true;

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->SaveState(saveLoadNode.child(item->data->name.GetString()));
		item = item->next;
	}
	saveLoadFile.save_file("save_game.xml");
	
	saveGameRequested = false;

	return ret;
}