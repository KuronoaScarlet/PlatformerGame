#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        int mouseMotionX, mouseMotionY;
        input->GetMouseMotion(mouseMotionX, mouseMotionY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {

                state = GuiControlState::PRESSED;

                if (bounds.x >= 143)
                {
                    bounds.x = mouseX - (bounds.w / 2);  // Slider
                    if (bounds.x + bounds.w <= 170 + 130)
                    {
                        bounds.x = mouseX - (bounds.w / 2);  // Slider
                    }
                    else
                    {
                        bounds.x = 168 + 130 - bounds.w;
                    }
                }
                else
                {
                    bounds.x = 145;
                }
                
                NotifyObserver();

            }
            NotifyObserver();
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::Draw(Render* render)
{
    if (!app->debugButton)
    {

        // Draw the right button depending on state
        switch (state)
        {
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255);
            break;
        case GuiControlState::NORMAL: render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::FOCUSED: render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);;
            break;
        case GuiControlState::PRESSED: render->DrawTexture(texturePressed, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (state)
        {
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255);
            break;
        case GuiControlState::NORMAL: render->DrawRectangle(bounds, 0, 255, 0, 255);
            break;
        case GuiControlState::FOCUSED: render->DrawRectangle(bounds, 255, 255, 0, 255);
            break;
        case GuiControlState::PRESSED: render->DrawRectangle(bounds, 0, 255, 255, 255);
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
            break;
        default:
            break;
        }
    }

    return false;
}
