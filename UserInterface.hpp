#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include "Button.hpp"
#include "Menu.hpp"

class UserInterface {

    private:
        Button button1;
        Button button2;
        Menu menu1;

    public:
        UserInterface();
        ~UserInterface();

        void render();
        void processMouseInput(const SDL_Event&);
};
