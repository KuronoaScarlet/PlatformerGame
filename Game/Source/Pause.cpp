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
#include "Pause.h"
#include "FadeToBlack.h"
#include "Title.h"


#include "Defs.h"
#include "Log.h"

Pause::Pause() : Module()
{
    name.Create("Logo");
}

Pause::~Pause()
{

}

bool Pause::Awake()
{
    LOG("Loading Logo");
    bool ret = true;

    return ret;
}

// Load assets
bool Pause::Start()
{
    LOG("Loading Logo assets");

    bool ret = true;

    screen = app->tex->Load("Assets/Textures/escape.png");

    resumeButton = new GuiButton(9, { 160, 67, 100, 24 }, "RESUME");
    resumeButton->SetObserver((Scene*)this);
    resumeButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/resume_button.png"), app->tex->Load("Assets/Textures/Buttons/resume_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/resume_button_pressed.png"));

    settingsButton = new GuiButton(2, { 160, 114, 100, 24 }, "SETTINGS");
    settingsButton->SetObserver((Scene*)this);
    settingsButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/settingns_button.png"), app->tex->Load("Assets/Textures/Buttons/settingns_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/settingns_button_pressed.png"));

    backToTitleButton = new GuiButton(11, { 160, 171, 100, 24 }, "BACK_TO_TITLE");
    backToTitleButton->SetObserver((Scene*)this);
    backToTitleButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/back_to_title_button.png"), app->tex->Load("Assets/Textures/Buttons/back_to_title_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/back_to_title_button_pressed.png"));

    exitButton = new GuiButton(4, { 160, 210, 100, 24 }, "EXIT");
    exitButton->SetObserver((Scene*)this);
    exitButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/exit_button.png"), app->tex->Load("Assets/Textures/Buttons/exit_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/exit_button_pressed.png"));

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    return ret;
}

bool Pause::PreUpdate()
{
    return true;
}

bool Pause::Update(float dt)
{
    resumeButton->Update(app->input, dt);
    settingsButton->Update(app->input, dt);
    backToTitleButton->Update(app->input, dt);
    exitButton->Update(app->input, dt);
    
    if (app->title->exi == true)
    {
        return false;
    }
    return true;
}

// Update: draw background
bool Pause::PostUpdate()
{
    bool ret = true;

   // Draw everything --------------------------------------
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->fade->Fade(this, (Module*)app->scene, 10);

    }
    //if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
    
    app->render->DrawTexture(screen, 0, 0, NULL);


    resumeButton->Draw(app->render);
    settingsButton->Draw(app->render);
    backToTitleButton->Draw(app->render);
    exitButton->Draw(app->render);
    
    return ret;
}

bool Pause::CleanUp()
{
    if (!active)return true;
    LOG("Freeing Logo");
    app->pause->active = false;
    app->tex->UnLoad(screen);
    return true;
}

bool Pause::OnGuiMouseClickEvent(GuiControl* control)
{
    

    return true;
}