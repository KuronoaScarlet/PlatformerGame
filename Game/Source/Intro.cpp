#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Enemies.h"
#include "Player.h"
#include "Animation.h"
#include "Intro.h"
#include "Scene.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

Intro::Intro() : Module()
{
    name.Create("Intro");
}

Intro::~Intro()
{

}

bool Intro::Awake()
{
    LOG("Loading Screens");
    bool ret = true;

    return ret;
}

// Load assets
bool Intro::Start()
{
    LOG("Loading Screens assets");

    bool ret = true;

    screen = app->tex->Load("Assets/Textures/title_screen.png");

    return ret;
}

bool Intro::PreUpdate()
{
    return true;
}

bool Intro::Update(float dt)
{
    

    return true;
}

// Update: draw background
bool Intro::PostUpdate()
{
    bool ret = true;
    app->render->camera.x = 0;
    app->render->camera.y = 0;
    // Draw everything --------------------------------------
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->fade->Fade(this, (Module*)app->scene, 60);
   
    }
    if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
    ret = false;
    app->render->DrawTexture(screen, 0, 0, NULL);
    
    return ret;
}

bool Intro::CleanUp()
{
    if (!active)return true;
    LOG("Freeing intro");
    app->intro->active = false;
    app->tex->UnLoad(screen);
    return true;
}