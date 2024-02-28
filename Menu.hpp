#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include "Button.hpp"
#include "Graphics.hpp"

class Menu {

    private:
        std::string title;
        SDL_Rect area;
        std::string texture;
        std::vector<Button> buttons;
        bool opened;
    
    public:
        Menu();
        ~Menu();

        void open();
        void close();
        void render() const;
        bool isOpen() const;
};
