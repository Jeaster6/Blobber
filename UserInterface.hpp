#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include "Button.hpp"
#include "ModalMenu.hpp"
#include "GameState.hpp"

class UserInterface {

    private:
        Button exitButton;
        ModalMenu modal;
        Button forward;
        Button backward;
        Button left;
        Button right;

    public:
        UserInterface();
        ~UserInterface();

        void render();
        int processMouseInput(const SDL_Event&, GameState&);
};
