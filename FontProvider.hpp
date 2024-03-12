#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "CurrentDirectory.hpp"

class FontProvider {

    private:
        SDL_Texture* font;
        SDL_Texture* text;

    public:
        FontProvider();
        FontProvider(SDL_Renderer*);
        ~FontProvider();
        
        void init(SDL_Renderer*);
        SDL_Texture* generateTextTexture(SDL_Renderer*, std::string& const, int, int, int);
};
