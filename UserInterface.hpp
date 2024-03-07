#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
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
        void addButton(const std::string&, const SDL_Rect&, const std::string&);
        void removeButton(const std::string&);
        void addMenu(const std::string&, const SDL_Rect&, const std::string&);
        Menu getMenuByTitle(const std::string&) const;
};
