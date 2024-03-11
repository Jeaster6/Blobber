#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include "Button.hpp"
#include "Graphics.hpp"

class Menu {

    protected:
        bool opened;
        SDL_Rect area;
        std::string texture;
    
    public:
        Menu();
        Menu(const SDL_Rect&, const std::string&);
        ~Menu();

        void open();
        void close();
        virtual void render() const = 0;
        bool isOpen() const;
};
