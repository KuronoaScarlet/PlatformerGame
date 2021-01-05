#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Player.h"
#include "Animation.h"
#include "WinScreen.h"
#include "Scene1.h"
#include "Scene2.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

WinScreen::WinScreen() : Module()
{
    name.Create("winscreen");
}

WinScreen::~WinScreen()
{

}

bool WinScreen::Awake()
{
    LOG("Loading Screens");
    bool ret = true;

    return ret;
}

// Load assets
bool WinScreen::Start()
{
    LOG("Loading Screens assets");

    bool ret = true;

    screen = app->tex->Load("Assets/Textures/win_screen.png");

    return ret;
}

bool WinScreen::PreUpdate()
{
    return true;
}

bool WinScreen::Update(float dt)
{
    return true;
}

// Update: draw background
bool WinScreen::PostUpdate()
{
    bool ret = true;
    // Draw everything --------------------------------------
    app->render->camera.x = 0;
    app->render->camera.y = 0;
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->player->playerData.playerLives = 3;
        app->fade->Fade(this, (Module*)app->intro, 60);
        app->scene1->firstEntry = true;
        app->scene2->firstEntry = true;
    }
   // if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
        
    app->render->DrawTexture(screen, 0, 0, NULL);

    return ret;
}

bool WinScreen::CleanUp()
{
    if (!active)return true;
    LOG("Freeing intro");
    app->winScreen->active = false;
    app->tex->UnLoad(screen);
    return true;
}