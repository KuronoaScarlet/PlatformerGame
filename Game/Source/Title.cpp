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
    start->SetTexture(app->tex->Load("Assets/Textures/Buttons/play.png"), app->tex->Load("Assets/Textures/Buttons/play_focused.png"),app->tex->Load("Assets/Textures/Buttons/play_pressed.png"));

    options = new GuiButton(2, { 170, 125, 80, 20 }, "OPTIONS");
    options->SetObserver((Scene*)this);
    options->SetTexture(app->tex->Load("Assets/Textures/Buttons/settings.png"), app->tex->Load("Assets/Textures/Buttons/settings_focused.png"), app->tex->Load("Assets/Textures/Buttons/settings_pressed.png"));

    exit = new GuiButton(4, { 170, 150, 80, 20 }, "EXIT");
    exit->SetObserver((Scene*)this);
    exit->SetTexture(app->tex->Load("Assets/Textures/Buttons/exit.png"), app->tex->Load("Assets/Textures/Buttons/exit_focused.png"), app->tex->Load("Assets/Textures/Buttons/exit_pressed.png"));

    fullSc = false;
    vsync = true;
    exi = false;
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
   // if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
    
    app->render->DrawTexture(screen, 0, 0, NULL);


   // start->Draw(app->render);
   // SDL_Rect rectPlayer = playerData.currentAnim->GetCurrentFrame();
    start->Draw(app->render);
    options->Draw(app->render);
    exit->Draw(app->render);

    if (exi == true) 
    {
        return false;
    }
    

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
        if (control->id == 1)
        {
            app->fade->Fade(this, (Module*)app->scene, 20);
        }
        else if (control->id == 2)
        {
            app->fade->Fade(this, (Module*)app->options, 10);
        }
        else if (control->id == 3)
        {
            app->fade->Fade(this, (Module*)app->title, 10);
        }
        else if (control->id == 4)
        {
            app->title->exi = true;
        }
        else if (control->id == 7)
        {
            if (app->title->fullSc == false)
            {
                SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
                app->title->fullSc = true;
            }
            else 
            {
                SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_RESIZABLE);
            }
            
        }
        else if (control->id == 8)
        {
            if (app->title->vsync == true)
            {
                
                app->title->vsync = false;
            }
            else
            {
               
            }
        }
        else if (control->id == 9)
        {
            app->audio->Volume(100);
        }
    }
    case GuiControlType::SLIDER:
    {
        if (control->id == 5)
        {
            if(control->bounds.x == 143 || control->bounds.x == 156.5f|| control->bounds.x == 170|| control->bounds.x == 183.5f|| control->bounds.x == 197|| control->bounds.x == 210.5f|| control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
            {
                if (control->bounds.x == 143)
                {
                    app->audio->Volume(0);
                }
                else
                {
                    app->title->volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
                    app->audio->Volume(app->title->volumMusic + 10);
                }
                
            }
            
        }
        else if (control->id == 6)
        {

        }

    }
    default: break;
    }

    return true;
}