#pragma once

#include <SDL.h>
#include <string>
#include <functional>
#include "Graphics.hpp"

class Button {

    private:
        SDL_Rect area;
        std::string texture;

    public:
        Button();
        Button(const SDL_Rect&, const std::string&);
        ~Button();

        bool click(int, int, int, int) const;
        void render() const;
};
