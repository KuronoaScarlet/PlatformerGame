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

    screen = app->tex->Load("Assets/Textures/options_screen.png");

    backButton = new GuiButton(3, { 10, 10, 20, 16 }, "BACK");
    backButton->SetObserver((Scene*)this);
    backButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/back_button.png"), app->tex->Load("Assets/Textures/Buttons/back_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/back_button_pressed.png"));

    musicMolume = new GuiSlider(5, { 143, 118, 20, 16 }, "MUSIC_VOLUME");
    musicMolume->SetObserver((Scene*)this);

    fxVolume = new GuiSlider(6, { 143, 157, 20, 16 }, "FX_VOLUME");
    fxVolume->SetObserver((Scene*)this);

    fullscreenButton = new GuiButton(7, { 210, 184, 20, 16 }, "FULLSCREEN");
    fullscreenButton->SetObserver((Scene*)this);
    fullscreenButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/fullscreen_button.png"), app->tex->Load("Assets/Textures/Buttons/fullscreen_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/fullscreen_button_pressed.png"));

    vsyncButton = new GuiButton(8, { 210, 206, 20, 16 }, "VSYNC");
    vsyncButton->SetObserver((Scene*)this);
    //vsyncButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/play.png"), app->tex->Load("Assets/Textures/Buttons/play_focused.png"), app->tex->Load("Assets/Textures/Buttons/play_pressed.png"));

   

    return ret;
}

bool Pause::PreUpdate()
{
    return true;
}

bool Pause::Update(float dt)
{
    backButton->Update(app->input, dt);
    musicMolume->Update(app->input, dt);
    fxVolume->Update(app->input, dt);
    fullscreenButton->Update(app->input, dt);
    vsyncButton->Update(app->input, dt);

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


    backButton->Draw(app->render);
    musicMolume->Draw(app->render);
    fxVolume->Draw(app->render);
    fullscreenButton->Draw(app->render);
    vsyncButton-> Draw(app->render);

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