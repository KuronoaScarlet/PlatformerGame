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
#include "Title.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

Title::Title() : Module()
{
    name.Create("Logo");
}

Title::~Title()
{

}

bool Title::Awake()
{
    LOG("Loading Logo");
    bool ret = true;

    return ret;
}

// Load assets
bool Title::Start()
{
    LOG("Loading Logo assets");

    bool ret = true;

    screen = app->tex->Load("Assets/Textures/title_screen.png");

    start = new GuiButton(1, { 170, 100, 80, 20 }, "START");
    start->SetObserver((Scene*)this);

    options = new GuiButton(2, { 170, 125, 80, 20 }, "OPTIONS");
    options->SetObserver((Scene*)this);

    exit = new GuiButton(3, { 170, 150, 80, 20 }, "EXIT");
    exit->SetObserver((Scene*)this);

    return ret;
}

bool Title::PreUpdate()
{
    return true;
}

bool Title::Update(float dt)
{
    start->Update(app->input, dt);
    options->Update(app->input, dt);
    exit->Update(app->input, dt);

    return true;
}

// Update: draw background
bool Title::PostUpdate()
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
    options->Draw(app->render);
    exit->Draw(app->render);

    return ret;
}

bool Title::CleanUp()
{
    if (!active)return true;
    LOG("Freeing Logo");
    app->title->active = false;
    app->tex->UnLoad(screen);
    return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 1)  app->fade->Fade(this, (Module*)app->scene, 10);
        else if (control->id == 2) app->fade->Fade(this, (Module*)app->options, 10);
        else if (control->id == 3) app->fade->Fade(this, (Module*)app->title, 10);
        else if (control->id == 4) return false;
    }
    default: break;
    }

    return true;
}