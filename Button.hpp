#pragma once

#include <SDL.h>
#include <string>
#include <functional>
#include "Graphics.hpp"

class Button {

    private:
        std::string title;
        SDL_Rect area;
        std::string texture;
        std::function<void()> action;

    public:
        Button();
        ~Button();

        void click();
        void render();
};
