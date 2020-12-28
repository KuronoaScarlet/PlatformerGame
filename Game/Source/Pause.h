#ifndef __PAUSE_H__
#define __PAUSE_H__

#include "Module.h"
#include "Animation.h"

#include "GuiButton.h"
#include "GuiSlider.h"

class GuiControl;


struct SDL_Texture;

class Pause : public Module
{
public:
    //Constructor
    Pause();

    //Destructor
    ~Pause();

    bool Awake();

    // Called when the module is activated
    // Loads the necessary textures for the map background
    bool Start();

    bool PreUpdate();

    // Called at the middle of the application loop
    // Updates the scene's background animations
    bool Update(float dt);

    // Called at the end of the application loop.
    // Performs the render call of all the parts of the scene's background
    bool PostUpdate();

    bool CleanUp();

    bool OnGuiMouseClickEvent(GuiControl* control);


public:
    // The scene sprite sheet loaded into an SDL_Texture
    SDL_Texture* screen = nullptr;

    GuiButton* resumeButton;
    GuiButton* settingsButton;
    GuiButton* backToTitleButton;
    GuiButton* exitButton;
 };

#endif