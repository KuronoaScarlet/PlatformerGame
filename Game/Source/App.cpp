#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Logo.h"
#include "Intro.h"
#include "Scene.h"
#include "Scene2.h"
#include "Map.h"
#include "Player.h"
#include "Enemies.h"
#include "CheckPoint.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "DeathScreen.h"


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
	intro = new Intro();
	scene = new Scene();
	scene2 = new Scene2();
	map = new Map();
	player = new Player();
	enemy = new Enemies();
	checkpoint = new CheckPoint();
	fade = new FadeToBlack();
	collisions = new Collisions(false);
	deathScreen = new DeathScreen();
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp

	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(logo);
	AddModule(intro);
	AddModule(scene);
	AddModule(scene2);
	AddModule(map);
	AddModule(player);
	AddModule(enemy);
	AddModule(checkpoint);
	AddModule(fade);
	AddModule(deathScreen);

	// Render last to swap buffer

	AddModule(collisions);
	AddModule(render);

	intro->active = false;
	scene->active = false;
	player->active = false;
	enemy->active = false;
	scene2->active = false;
	checkpoint->active = false;
	deathScreen->active = false;
	//collisions->active = false;
	PERF_PEEK(perfTimer);
}

// Destructor
App::~App()
{
	// Release modules
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

// Called before render is available
bool App::Awake()
{
	PERF_START(perfTimer);
	// TODO 3: Load config from XML

	bool ret = LoadConfig();

	if (ret == true)
	{
		// TODO 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		win->SetTitle(title.GetString());

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// TODO 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	pugi::xml_parse_result result = saveLoadFile.load_file("savegame.xml");
	if (result != NULL)
	{
		saveLoadNode = saveLoadFile.child("save");
	}
	frameRate = configApp.attribute("framerate_cap").as_int(0);
	PERF_PEEK(perfTimer);
	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(perfTimer);
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if(item->data->active == true)
			ret = item->data->Start();

		item = item->next;
	}
	PERF_PEEK(perfTimer);
	return ret;
}

// Called each loop iteration
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

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = true;

	// TODO 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result result = configFile.load_file("config.xml");

	// TODO 3: Check result for loading errors
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
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// Load / Save Calls
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	PERF_START(perfTimer);
	uint32 lastFrameInMs = 0;
	uint32 framesOnLastUpdate = 0;

	float secondsStart = startTime.ReadSec();
	float average = fpsCount / secondsStart;

	if (frameTime.ReadSec() > 1.0f)
	{
		framesSecond = lastSecFrameCnt;
		lastSecFrameCnt = 0;
		frameTime.Start();
	}

	oldLastFrame = lastFrameInMs;

	lastFrameInMs = lastSecond.Read();

	lastSecond.Start();

	int delay = (1000 * (1.0f / frameRate));

	if (lastFrameInMs < 1000 * (1.0f / frameRate))
	{
		perfTimer.Start();
		SDL_Delay(delay);
		timePerfect = perfTimer.ReadMs();
		LOG("We waited for %d milliseconds and got back in %f milliseconds", delay, timePerfect);
	}

	static char title[256];
	/*sprintf_s(title, 256, "Game (Lost by TDM Studios) Av.FPS: %.2f", average);

	app->win->SetTitle(title);*/
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
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
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

// L02: TODO 7: Implement the xml save method for current state
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
	saveLoadFile.save_file("savegame.xml");

	saveGameRequested = false;

	return ret;
}