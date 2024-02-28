#pragma once

#include <vector>
#include <iostream>
#include "Button.hpp"
#include "Menu.hpp"

class UserInterface {

    private:
        std::vector<Button> buttons;
        std::vector<Menu> menus;

    public:
        UserInterface();
        ~UserInterface();

        void render();
        void processMouseInput(const SDL_Event&);
};
