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

    public:
        Button();
        Button(const std::string&, const SDL_Rect&, const std::string&);
        ~Button();

        bool click(int, int, int, int) const;
        void render() const;
        std::string getTitle() const;
};
