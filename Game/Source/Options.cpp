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
#include "Options.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

Options::Options() : Module()
{
    name.Create("Logo");
}

Options::~Options()
{

}

bool Options::Awake()
{
    LOG("Loading Logo");
    bool ret = true;

    return ret;
}

// Load assets
bool Options::Start()
{
    LOG("Loading Logo assets");

    bool ret = true;

    screen = app->tex->Load("Assets/Textures/options_screen.png");

   start = new GuiButton(1, { 170, 100, 80, 20 }, "START");
    start->SetObserver((Scene*)this);

    exit = new GuiButton(2, { 170, 125, 80, 20 }, "EXIT");
    exit->SetObserver((Scene*)this);

    return ret;
}

bool Options::PreUpdate()
{
    return true;
}

bool Options::Update(float dt)
{
    start->Update(app->input, dt);
    exit->Update(app->input, dt);

    return true;
}

// Update: draw background
bool Options::PostUpdate()
{
    bool ret = true;

   // Draw everything --------------------------------------
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->fade->Fade(this, (Module*)app->scene, 10);

    }
    if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
    ret = false;
    app->render->DrawTexture(screen, 0, 0, NULL);


    start->Draw(app->render);
    exit->Draw(app->render);

    return ret;
}

bool Options::CleanUp()
{
    if (!active)return true;
    LOG("Freeing Logo");
    app->options->active = false;
    app->tex->UnLoad(screen);
    return true;
}

bool Options::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 1)  app->fade->Fade(this, (Module*)app->scene, 10);
        else if (control->id == 2) return false;
    }
    default: break;
    }

    return true;
}