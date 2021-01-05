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
#include "Player.h"


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

    play = new GuiButton(1, { 170, 75, 80, 20 }, "START");
    play->SetObserver((Scene*)this);
    play->SetTexture(app->tex->Load("Assets/Textures/Buttons/play.png"), app->tex->Load("Assets/Textures/Buttons/play_focused.png"),app->tex->Load("Assets/Textures/Buttons/play_pressed.png"));

    continueButton = new GuiButton(12, { 170, 105, 80, 20 }, "START");
    continueButton->SetObserver((Scene*)this);
    continueButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/load_button.png"), app->tex->Load("Assets/Textures/Buttons/load_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/load_button_pressed.png"));

    options = new GuiButton(2, { 170, 135, 80, 20 }, "OPTIONS");
    options->SetObserver((Scene*)this);
    options->SetTexture(app->tex->Load("Assets/Textures/Buttons/settings.png"), app->tex->Load("Assets/Textures/Buttons/settings_focused.png"), app->tex->Load("Assets/Textures/Buttons/settings_pressed.png"));

    credits = new GuiButton(13, { 170, 165, 80, 20 }, "OPTIONS");
    credits->SetObserver((Scene*)this);
    credits->SetTexture(app->tex->Load("Assets/Textures/Buttons/credits_button.png"), app->tex->Load("Assets/Textures/Buttons/credits_button_focused.png"), app->tex->Load("Assets/Textures/Buttons/credits_button_pressed.png"));

    exit = new GuiButton(4, { 170, 195, 80, 20 }, "EXIT");
    exit->SetObserver((Scene*)this);
    exit->SetTexture(app->tex->Load("Assets/Textures/Buttons/exit.png"), app->tex->Load("Assets/Textures/Buttons/exit_focused.png"), app->tex->Load("Assets/Textures/Buttons/exit_pressed.png"));

   
    pauseBool = false;
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
    play->Update(app->input, dt);
    continueButton->Update(app->input, dt);
    options->Update(app->input, dt);
    credits->Update(app->input, dt);
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
    play->Draw(app->render);
    continueButton->Draw(app->render);
    options->Draw(app->render);
    credits->Draw(app->render);
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
            //Play
            app->fade->Fade(this, (Module*)app->scene, 20);          
        }
        else if (control->id == 2)
        {
            //Options
            if(app->player->pauseCondition)    app->fade->Fade((Module*)app->scene, (Module*)app->options, 10);
            else app->fade->Fade(this, (Module*)app->options, 10);
        }
        else if (control->id == 3)
        {
            //Back
            if (app->player->pauseCondition == false)
            {
                app->fade->Fade((Module*)app->options, (Module*)app->title, 10);
            }
            else
            {
                app->fade->Fade((Module*)app->options, (Module*)app->title, 10);
            }
        }
        else if (control->id == 11)
        {
            //Back to title
            if(app->player->scene1) app->fade->Fade((Module*)app->scene, (Module*)app->title, 10);
            if (app->player->scene2) app->fade->Fade((Module*)app->scene2, (Module*)app->title, 10);

        }

        else if (control->id == 4)
        {
            //Exit
            app->title->exi = true;
        }
       else if (control->id == 8)
        {
            //Vsync
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
            //resume
            app->player->pauseCondition = false;
            
        }
        else if (control->id == 12)
        {
            //LoadGame
            app->LoadGameRequest();
        }
    }
    case GuiControlType::SLIDER:
    {
        if (control->id == 5)
        {
            //Volume
            if(control->bounds.x == 143 || control->bounds.x == 156.5f|| control->bounds.x == 170|| control->bounds.x == 183.5f|| control->bounds.x == 197|| control->bounds.x == 210.5f|| control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
            {
                app->title->volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
                app->audio->Volume(app->title->volumMusic + 10, '0');
                
                
            }
            
        }
        else if (control->id == 6)
        {
            //FxVolume
            if (control->bounds.x == 143 || control->bounds.x == 156.5f || control->bounds.x == 170 || control->bounds.x == 183.5f || control->bounds.x == 197 || control->bounds.x == 210.5f || control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
            {
                if (control->bounds.x == 143)
                {
                    app->audio->Volume(0, '1');
                }
                else
                {
                    app->title->volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
                    app->audio->Volume(app->title->volumMusic + 10, '1');
                }

            }

        }

    }
    case GuiControlType::CHECKBOX:
    {
        if (control->id == 7)
        {
            //FullScreen
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
    }
    default: break;
    }

    return true;
}