#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "CurrentDirectory.hpp"

class FontProvider {

    private:
        SDL_Texture* fontTexture;
        SDL_Texture* textTexture;

    public:
        FontProvider();
        ~FontProvider();
        
        void init(SDL_Renderer*);
        SDL_Texture* generateTextTexture(SDL_Renderer*, const std::string&, int, int, int, int);
};
