#ifndef __FONTS_H__
#define __FONTS_H__

#include "Defs.h"

#include "SDL/include/SDL.h"

class Textures;

class Fonts
{
public:

    Fonts(const char* rtpFontFile, SDL_Texture* tex);

    ~Fonts();

    SDL_Texture* GetTextureAtlas();
    SDL_Rect GetCharRec(int value);

private:

    bool fontLoaded;

    SDL_Texture* texture;

    int baseSize;
    int charsCount;
    SDL_Rect charsRecs[128];
};

#endif //__FONT_H__