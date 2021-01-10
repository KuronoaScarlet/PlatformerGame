#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Animation.h"
#include "DeathScreen.h"
#include "Scene1.h"
#include "Scene2.h"
#include "FadeToBlack.h"
#include "EntityManager.h"


#include "Defs.h"
#include "Log.h"

DeathScreen::DeathScreen() : Module()
{
    name.Create("deathscreen");
}

DeathScreen::~DeathScreen()
{

}

bool DeathScreen::Awake()
{
    LOG("Loading Screens");
    bool ret = true;

    return ret;
}

// Load assets
bool DeathScreen::Start()
{
    LOG("Loading Screens assets");

    bool ret = true;

    screen = app->tex->Load("Assets/Textures/death_screen.png");
    app->audio->PlayMusic("Assets/Audio/Music/death_scene_music.ogg");

    timer = 0;
    trans = true;

    return ret;
}

bool DeathScreen::PreUpdate()
{
    return true;
}

bool DeathScreen::Update(float dt)
{
    timer += dt;
    return true;
}

// Update: draw background
bool DeathScreen::PostUpdate()
{
    bool ret = true;
    // Draw everything --------------------------------------
    app->render->camera.x = 0;
    app->render->camera.y = 0;
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->entityManager->playerData.lives = 3;
        app->fade->Fade(this, (Module*)app->intro, 1);
    }
    
    app->render->DrawTexture(screen, 0, 0, NULL);

    return ret;
}

bool DeathScreen::CleanUp()
{
    if (!active)return true;
    LOG("Freeing intro");
    app->deathScreen->active = false;
    app->tex->UnLoad(screen);
    return true;
}